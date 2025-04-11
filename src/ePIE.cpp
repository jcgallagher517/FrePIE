#include <vector>
#include <cmath>
#include <xtensor/containers/xarray.hpp>

#include "ePIE.hpp"


// include main ePIE implementation here
std::vector<double> ePIE(xt::xarray<std::complex<double>> obj,
                         xt::xarray<std::complex<double>> prb,
                         xt::xarray<double> dps,
                         xt::xarray<int> scan_pos,
                         double obj_step, double prb_step, int n_iters) {


  // int n_dps = dps.shape(0);
  // if (scan_pos.shape(0) != n_dps) {
  //   throw std::runtime_error("# of scan positions must equal # of dps");
  // }

  // int error_per_iter, x_pos, y_pos;
  // for (int iter = 1; iter <= n_iters; ++iter) {
  //   error_per_iter = 0;
  //   for (int k = 0; k < n_dps; ++k) {
  //     x_pos = scan_pos[k][0];
  //     y_pos = scan_pos[k][1];
  //   }
  // }

  std::vector<double> errors(n_iters);
  for (auto i : errors) {
    i++;
    errors[i] = static_cast<double>(pow(2, i));
  }

  return errors;

}
