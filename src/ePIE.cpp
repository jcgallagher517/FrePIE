#include "ePIE.hpp"
#include "FFT2.hpp"
#include <Eigen/Dense>
#include <assert.h>
#include <complex>
#include <iostream>
#include <cmath>
#include <fftw3.h>
#include <vector>
#include <chrono>

namespace eig = Eigen;
using ArrayXcdRM = eig::Array<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
using ArrayXdRM = eig::Array<double, eig::Dynamic, eig::Dynamic, eig::RowMajor>;

std::vector<double> ePIE(ArrayXcdRM &obj, ArrayXcdRM &prb,
                         const std::vector<ArrayXdRM> &dps,
                         const std::vector<std::vector<int>> &scan_pos,
                         double obj_step, double prb_step, int n_iters) {

  unsigned int n_dps = dps.size();
  if (scan_pos.size() != n_dps) {
    throw std::runtime_error("# of scan positions must equal # of dps");
  }

  // FFT routine for objects shaped like probe
  int prb_dx = prb.rows(), prb_dy = prb.cols();
  assert(prb_dx == dps[0].rows() && prb_dy == dps[0].cols());
  FFT2 fft(prb_dx, prb_dy, FFTW_FORWARD);
  FFT2 ifft(prb_dx, prb_dy, FFTW_BACKWARD);

  std::vector<double> errors(n_iters);
  double error_per_iter, error_norm = 0;
  for (size_t k = 0; k < n_dps; ++k) {
    error_norm += dps[k].abs2().sum();
  }

  int x_pos, y_pos;
  ArrayXcdRM psi(prb_dx, prb_dy);
  ArrayXcdRM psi_k(prb_dx, prb_dy);
  ArrayXcdRM psi_k_p(prb_dx, prb_dy);
  ArrayXcdRM psi_p(prb_dx, prb_dy);
  ArrayXcdRM d_psi(prb_dx, prb_dy);
  ArrayXcdRM lil_obj(prb_dx, prb_dy);

  std::cout << "Commencing reconstruction...\n";
  for (int iter = 1; iter <= n_iters; ++iter) {

    auto iter_start = std::chrono::high_resolution_clock::now();
    error_per_iter = 0;

    for (size_t k = 0; k < n_dps; ++k) {
      x_pos = scan_pos[k][0];
      y_pos = scan_pos[k][1];

      lil_obj = obj.block(x_pos, y_pos, prb_dx, prb_dy);

      // exit wave and its Fourier transform
      psi = lil_obj * prb;
      fft.compute(psi, psi_k);

      // replace modulus with (amplitude of..) diffraction pattern
      // assert(dps[k].rows() == psi_k.rows() && dps[k].cols() == psi_k.cols());
      psi_k_p = dps[k] * psi_k / psi_k.abs();
      ifft.compute(psi_k_p, psi_p);

      // update obj, prb, error
      d_psi = psi_p - psi;

      obj.block(x_pos, y_pos, prb_dx, prb_dy) = lil_obj + obj_step *
        d_psi * prb.conjugate() / prb.abs2().maxCoeff();

      prb = prb + prb_step *
        d_psi * lil_obj.conjugate() / lil_obj.abs2().maxCoeff();

      error_per_iter += d_psi.abs2().mean();

    }
    
    auto iter_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = iter_end - iter_start;
    std::cout << "Iteration " << iter
              << ", Error: " << error_per_iter
              << ", Time: " << duration.count() << "s\n";
    errors[iter - 1] = error_per_iter / error_norm;

  }

  return errors;
}
