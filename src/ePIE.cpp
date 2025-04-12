#include <vector>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>

#include "ePIE.hpp"

namespace eig = Eigen;

// include main ePIE implementation here
std::vector<double> ePIE(eig::MatrixXcd obj,
                         eig::MatrixXcd prb,
                         const std::vector<eig::MatrixXd>& dps,
                         const eig::MatrixXi& scan_pos,
                         double obj_step, double prb_step, int n_iters) {
  


  int n_dps = dps.size();
  std::cout << n_dps << '\n';
  if (scan_pos.rows() != n_dps) {
    throw std::runtime_error("# of scan positions must equal # of dps");
  }

  // take fourier transform of object just as a test

  // main loops, do stuff here later
  double error_per_iter;
  std::vector<double> errors(n_iters);
  int x_pos, y_pos;
  for (int iter = 1; iter <= n_iters; ++iter) {
    error_per_iter = 0;
    for (int k = 0; k < n_dps; ++k) {
      x_pos = scan_pos(k, 0);
      y_pos = scan_pos(k, 1);

      if (iter == 1) {

        std::cout << x_pos << " " << y_pos << "\n";
        
      }

      // rms error per diffraction pattern

    }

    // calculate rms error
    error_per_iter = static_cast<double>(x_pos * y_pos);
    errors[iter-1] = error_per_iter;

  }
  return errors;

}
