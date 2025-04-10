#include <complex>
#include <iostream> // for debugging for now
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <stdexcept>
#include <xtensor/containers/xarray.hpp>
#include <xtensor/containers/xtensor.hpp>


namespace py = pybind11;

auto ePIE(py::array_t<std::complex<double>>& obj, py::array_t<std::complex<double>>& prb,
          const py::array_t<double>& dps, const py::array_t<int>& scan_pos,
          double obj_step, double prb_step, int n_iters) {
  /* ePIE ptychographic reconstruction routine
     @params:
         obj: initialized (guess) object to be reconstructed, shape: (bigX, bigY), MUTATED BY ePIE
         prb: initialized (guess) probe to be resconstructed, shape: (lilX, lilY),  MUTATED BY ePIE
         dps: diffraction patterns, shape: (k, lilX, lilY)
         scan_pos: scan positions (in pixels) for each diffraction pattern, shape: (k, 2)
         obj_step, prb_step, n_iters: hyperparameters
     @return:
         errors: RMS error of all dps for each iteration, shape: (n_iters)
   */


  py::buffer_info obj_buf = obj.request();
  py::buffer_info prb_buf = prb.request(); // why works for object but not probe?


  int n_dps = dps.shape(0);
  if (scan_pos.shape(0) != n_dps) {
    throw std::runtime_error("# of scan positions must equal # of dps");
  }

  int error_per_iter, x_pos, y_pos;
  for (int iter = 1; iter <= n_iters; ++iter) {
    error_per_iter = 0;
    for (int k = 0; k < n_dps; ++k) {

      x_pos = scan_pos[k][0];
      y_pos = scan_pos[k][1];
      

    }
  }


  return 0;
}

PYBIND11_MODULE(FrePIE, m) {

  m.def("ePIE", &ePIE, "Robust iterative ptychography algorithm",
        py::arg("obj"), py::arg("prb"),
        py::arg("dps"), py::arg("scan_pos"),
        py::arg("obj_step"), py::arg("prb_step"), py::arg("n_iters"));

}
