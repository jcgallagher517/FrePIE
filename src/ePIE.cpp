#include <pybind11/pybind11.h>

double add(double a, double b) {
  return a + b;
}

PYBIND11_MODULE(ePIE, m) {

  m.def("add", &add, "A function that adds",
        pybind11::arg("a"), pybind11::arg("b"));

}
