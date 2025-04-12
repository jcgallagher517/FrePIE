#include <complex>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <Eigen/Dense>

#include "ePIE.hpp"

namespace py = pybind11;
namespace eig = Eigen;

// cast the py::array_t objects as Eigen matrices
template <typename T> eig::Matrix<T, eig::Dynamic, eig::Dynamic> numpy_to_eigen(py::array_t<T> array) {

  py::buffer_info buf = array.request();
  if (buf.ndim != 2) {
    throw std::runtime_error("NumPy array must be 2D");
  }

  auto rows = static_cast<eig::Index>(buf.shape[0]);
  auto cols = static_cast<eig::Index>(buf.shape[1]);

  using MatrixType = eig::Matrix<T, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
  return eig::Map<MatrixType>(static_cast<T*>(buf.ptr), rows, cols);
}


// ad hoc turn dps into a vector of Eigen matrices for each dp
std::vector<eig::MatrixXd> process_dps(py::array_t<double> dps) {

  py::buffer_info buf = dps.request();

  int n_dps = buf.shape[0];
  ssize_t rows = buf.shape[1];
  ssize_t cols = buf.shape[2];

  std::vector<eig::MatrixXd> new_dps;
  new_dps.reserve(n_dps);

  // un-flatten data, every row*col is a new dp
  double* data = static_cast<double*>(buf.ptr);
  using MatrixType = eig::Matrix<double, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
  for (ssize_t k = 0; k < n_dps; ++k) {
    new_dps.emplace_back(eig::Map<MatrixType>(data + k*rows*cols, rows, cols));
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
  std::vector<double> errors = ePIE(numpy_to_eigen(obj),
                                    numpy_to_eigen(prb),
                                    process_dps(dps),
                                    numpy_to_eigen(scan_pos),
                                    obj_step, prb_step, n_iters);

  auto result = py::array_t<double>(errors.size());
  std::copy(errors.begin(), errors.end(), result.mutable_data());
  return result;
}


// finally, define module to export to Python
PYBIND11_MODULE(FrePIElibcpp, m) {

  m.def("ePIE", &ePIE_wrapper, "Robust iterative ptychography algorithm",
        py::arg("obj"), py::arg("prb"),
        py::arg("dps"), py::arg("scan_pos"),
        py::arg("obj_step"), py::arg("prb_step"), py::arg("n_iters"));

}
