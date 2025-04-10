#include <pybind11/pybind11.h>

namespace py = pybind11;

double add(double a, double b) {
  return a + b;
}

PYBIND11_MODULE(ePIE, m) {

  m.def("add", &add, "A function that adds",
        py::arg("a"), py::arg("b"));

}
