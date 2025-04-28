#include <complex>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <Eigen/Dense>
#include "ePIE.hpp"

namespace py = pybind11;
namespace eig = Eigen;

using ArrayXcdRM = eig::Array<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
using ArrayXdRM = eig::Array<double, eig::Dynamic, eig::Dynamic, eig::RowMajor>;

// cast the py::array_t objects as Eigen arrays
ArrayXcdRM numpy_to_eigen(py::array_t<std::complex<double>> array) {

  py::buffer_info buf = array.request();
  if (buf.ndim != 2) {
    throw std::runtime_error("NumPy array must be 2D");
  }
  int rows = buf.shape[0];
  int cols = buf.shape[1];
  return eig::Map<ArrayXcdRM>(static_cast<std::complex<double>*>(buf.ptr), rows, cols);
}


// wrapper function to call ePIE implementation as defined in ePIE.cpp
py::array_t<double> ePIE_wrapper(py::array_t<std::complex<double>> obj,
                                 py::array_t<std::complex<double>> prb,
                                 const py::array_t<double>& dps,
                                 const py::array_t<int>& scan_pos,
                                 double obj_step, double prb_step, int n_iters)
{

  // cast object and probe as eigen arrays
  ArrayXcdRM eigen_obj = numpy_to_eigen(obj);
  ArrayXcdRM eigen_prb = numpy_to_eigen(prb);
  
  // cast scan_pos as vec of vecs
  py::buffer_info scan_pos_buf = scan_pos.request();
  int n_dps = scan_pos_buf.shape[0];
  assert(scan_pos_buf.ndim == 2 && scan_pos_buf.shape[1] == 2);
  int* scan_pos_ptr = static_cast<int*>(scan_pos_buf.ptr);
  std::vector<std::vector<int>> cpp_scan_pos(n_dps);

  // cast dps as vec of Eigen::ArrayXdRM
  py::buffer_info dps_buf = dps.request();
  assert(n_dps = dps_buf.shape[0]); // should be as many dps as scan positions
  double* dps_ptr = static_cast<double*>(dps_buf.ptr);
  int rows = dps_buf.shape[1];
  int cols = dps_buf.shape[2];
  std::vector<ArrayXdRM> cpp_dps(n_dps);

  // populate both dps and scan_pos
  for (int k = 0; k < scan_pos_buf.shape[0]; ++k) {
    cpp_scan_pos[k] = { scan_pos_ptr[2*k], scan_pos_ptr[2*k+1] };
    cpp_dps[k] = eig::Map<ArrayXdRM>(dps_ptr + k * rows * cols, rows, cols);
  }

  // only eigen_obj and eigen_prb are passed by reference
  std::vector<double> errors = ePIE(eigen_obj,
                                    eigen_prb,
                                    cpp_dps,
                                    cpp_scan_pos,
                                    obj_step, prb_step, n_iters);

  // copy data from eigen_obj/eigen_prb back to python data pointers
  py::buffer_info obj_buf = obj.request();
  py::buffer_info prb_buf = prb.request();
  std::memcpy(obj_buf.ptr, eigen_obj.data(), sizeof(std::complex<double>) * eigen_obj.size());
  std::memcpy(prb_buf.ptr, eigen_prb.data(), sizeof(std::complex<double>) * eigen_prb.size());

  // return errors as array
  auto result = py::array_t<double>(errors.size());
  std::copy(errors.begin(), errors.end(), result.mutable_data());
  return result;
}

PYBIND11_MODULE(libFrePIE, m) {
  m.def("ePIE", &ePIE_wrapper, "Robust iterative ptychography algorithm",
        py::arg("obj"), py::arg("prb"),
        py::arg("dps"), py::arg("scan_pos"),
        py::arg("obj_step"), py::arg("prb_step"), py::arg("n_iters"));
}
