#include <complex>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


py::array_t<double> ePIE(py::array_t<std::complex<double>> obj, py::array_t<std::complex<double>> prb,
                         py::array_t<double> dps, py::array_t<int> scan_pos,
                         double obj_step, double prb_step, int n_iters) {


  auto result = py::array_t<double>(obj.size());
  return result;

}

PYBIND11_MODULE(FrePIE, m) {

  m.def("ePIE", &ePIE, "Robust iterative ptychography algorithm",
        py::arg("obj"), py::arg("prb"),
        py::arg("dps"), py::arg("scan_pos"),
        py::arg("obj_step"), py::arg("prb_step"), py::arg("n_iters"));

}
