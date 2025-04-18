#include <complex>
#include <vector>
#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <fftw3.h>

#include <matplot/matplot.h>

#include "ePIE.hpp"

namespace eig = Eigen;


// main ePIE implementation here

// import error mentioning Eigen Matrices arose when I started passing by reference
// but I have to pass by reference in order for thing to be changed, unless I return tuples
std::vector<double> ePIE(eig::Ref<eig::MatrixXcd> obj,
                         eig::Ref<eig::MatrixXcd> prb,
                         const std::vector<eig::MatrixXd>& dps,
                         const eig::MatrixXi& scan_pos,
                         double obj_step, double prb_step, int n_iters) {
  
  int n_dps = dps.size();
  if (scan_pos.rows() != n_dps) {
    throw std::runtime_error("# of scan positions must equal # of dps");
  }

  // take in-place fourier transform of probe and ifft of object
  // just as a test that they both work

  FFT_2D fft(prb.rows(), prb.cols(), FFTW_FORWARD);
  fft.compute(prb, prb);

  FFT_2D ifft(obj.rows(), obj.cols(), FFTW_BACKWARD);
  ifft.compute(obj, obj);


  // display as images


  



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
