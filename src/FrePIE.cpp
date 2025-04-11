#include <complex>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <xtensor.hpp>
#include <xtensor/containers/xadapt.hpp>

#include "ePIE.hpp"

namespace py = pybind11;

template <typename T> xt::xarray<T> pyarray_to_xarray(py::array_t<T> arr) {

  py::buffer_info buf = arr.request();
  T* data = static_cast<T*>(buf.ptr);
  return xt::adapt(data, buf.size, xt::no_ownership(), buf.shape);
    
  //   // pointer to raw data
  //   T* ptr = static_cast<T*>(buf.ptr);
  //   // convert shape and strides from Python buffer info
  //   std::vector<std::size_t> shape(buf.shape.begin(), buf.shape.end());
  // 
  //   // convert strides from bytes to element strides
  //   std::vector<std::size_t> strides;
  //   for (auto s : buf.strides) {
  //     strides.push_back(s / sizeof(T));
  //   }
  // 
  //   // wrap with xt::adapt - no copy, shared buffer
  //   return xt::adapt(ptr, buf.size, xt::no_ownership(), shape, strides);
  
}

// MAKE SURE TO USE np.ascontiguousarray() in wrapper function in Python

py::array_t<double> ePIE_wrapper(py::array_t<std::complex<double>> obj,
                                 py::array_t<std::complex<double>> prb,
                                 py::array_t<double> dps,
                                 py::array_t<int> scan_pos,
                                 double obj_step, double prb_step, int n_iters)
{
  std::vector<double> errors = ePIE(pyarray_to_xarray(obj),
                                    pyarray_to_xarray(prb),
                                    pyarray_to_xarray(dps),
                                    pyarray_to_xarray(scan_pos),
                                    obj_step, prb_step, n_iters);

  return py::array_t<double>(errors.size(), errors.data());
}

PYBIND11_MODULE(FrePIE, m) {

  m.def("ePIE", &ePIE_wrapper, "Robust iterative ptychography algorithm",
        py::arg("obj"), py::arg("prb"),
        py::arg("dps"), py::arg("scan_pos"),
        py::arg("obj_step"), py::arg("prb_step"), py::arg("n_iters"));

}
