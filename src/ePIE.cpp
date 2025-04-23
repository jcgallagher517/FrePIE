#include <complex>
#include <vector>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <fftw3.h>

#include "ePIE.hpp"
#include "FFT2.hpp"

namespace eig = Eigen;

using MatXcdRM = eig::Matrix<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
std::vector<double> ePIE(MatXcdRM& obj,
                         MatXcdRM& prb,
                         const std::vector<eig::MatrixXd>& dps,
                         const eig::MatrixXi& scan_pos,
                         double obj_step, double prb_step, int n_iters) {
  
  int n_dps = dps.size();
  if (scan_pos.rows() != n_dps) {
    throw std::runtime_error("# of scan positions must equal # of dps");
  }

  // take in-place ifft of probe and fft of object
  // just as a test that they both work

  FFT2 ifft(prb.rows(), prb.cols(), FFTW_BACKWARD);
  ifft.compute(prb, prb);

  FFT2 fft(obj.rows(), obj.cols(), FFTW_FORWARD);
  fft.compute(obj, obj);

  // main loops, do stuff here later
  double error_per_iter;
  std::vector<double> errors(n_iters);
  int x_pos, y_pos;
  // for (int iter = 1; iter <= n_iters; ++iter) {
  //   error_per_iter = 0;
  //   for (int k = 0; k < n_dps; ++k) {
  //     x_pos = scan_pos(k, 0);
  //     y_pos = scan_pos(k, 1);
  //     // rms error per diffraction pattern
  //   }
  //   // calculate rms error
  //   error_per_iter = static_cast<double>(x_pos * y_pos);
  //   errors[iter-1] = error_per_iter;
  // }

  return errors;

}
