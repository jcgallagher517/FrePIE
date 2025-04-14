#include <complex>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
// #include <pybind11/stl.h>
#include <Eigen/Dense>
#include "ePIE.hpp"

#include <iostream>

namespace py = pybind11;
namespace eig = Eigen;

// cast the py::array_t objects as Eigen matrices
template <typename T>
eig::Matrix<T, eig::Dynamic, eig::Dynamic>
numpy_to_eigen(py::array_t<T> array) {

  py::buffer_info buf = array.request();
  if (buf.ndim != 2) {
    throw std::runtime_error("NumPy array must be 2D");
  }
  int rows = buf.shape[0];
  int cols = buf.shape[1];
  using MatrixType = eig::Matrix<T, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
  return eig::Map<MatrixType>(static_cast<T*>(buf.ptr), rows, cols);
}


// ad hoc turn dps into a vector of Eigen matrices for each dp
std::vector<eig::MatrixXd> dps_to_eigen(py::array_t<double> dps) {

  // collect info from numpy array
  py::buffer_info buf = dps.request();
  double* data = static_cast<double*>(buf.ptr);
  int n_dps = buf.shape[0];
  int rows = buf.shape[1];
  int cols = buf.shape[2];

  // un-flatten data, every row*col is the next dp
  std::vector<eig::MatrixXd> new_dps(n_dps);
  using MatrixType = eig::Matrix<double, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
  for (int k = 0; k < n_dps; ++k) {
    new_dps[k] = eig::Map<MatrixType>(data + k * rows * cols, rows, cols);
  }
  return new_dps;
}

// wrapper function to call ePIE implementation as defined in ePIE.cpp
py::array_t<double> ePIE_wrapper(py::array_t<std::complex<double>> obj,
                                 py::array_t<std::complex<double>> prb,
                                 const py::array_t<double>& dps,
                                 const py::array_t<int>& scan_pos,
                                 double obj_step, double prb_step, int n_iters)
{

  // cast object and probe as eigen matrices
  using MatXcdRM = eig::Matrix<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
  MatXcdRM eigen_obj = numpy_to_eigen(obj);
  MatXcdRM eigen_prb = numpy_to_eigen(prb);

  std::vector<double> errors = ePIE(eigen_obj, eigen_prb,
                                    dps_to_eigen(dps),
                                    numpy_to_eigen(scan_pos),
                                    obj_step, prb_step, n_iters);

  // convert object and probe data back to numpy-compatible forms
  py::buffer_info obj_buf, prb_buf;
  obj_buf = obj.request();
  prb_buf = prb.request();
  obj_buf.ptr = eigen_obj.data();
  prb_buf.ptr = eigen_prb.data();


  auto result = py::array_t<double>(errors.size());
  std::copy(errors.begin(), errors.end(), result.mutable_data());
  return result; // look into returning a dict
}

PYBIND11_MODULE(libFrePIE, m) {
  m.def("ePIE", &ePIE_wrapper, "Robust iterative ptychography algorithm",
        py::arg("obj"), py::arg("prb"),
        py::arg("dps"), py::arg("scan_pos"),
        py::arg("obj_step"), py::arg("prb_step"), py::arg("n_iters"));
}
