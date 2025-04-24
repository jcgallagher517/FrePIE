#include "ePIE.hpp"
#include "FFT2.hpp"
#include <Eigen/Dense>
#include <assert.h>
#include <complex>
#include <cmath>
#include <fftw3.h>
#include <vector>

namespace eig = Eigen;
using MatXcdRM = eig::Matrix<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;
using ArrayXcdRM = eig::Array<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;

std::vector<double> ePIE(MatXcdRM &obj, MatXcdRM &prb,
                         const std::vector<eig::MatrixXd> &dps,
                         const eig::MatrixXi &scan_pos, double obj_step,
                         double prb_step, int n_iters) {

  int n_dps = dps.size();
  if (scan_pos.rows() != n_dps) {
    throw std::runtime_error("# of scan positions must equal # of dps");
  }

  // FFT routine for objects shaped like probe
  int prb_dx = prb.rows(), prb_dy = prb.cols();
  assert(prb_dx == dps[0].rows() && prb_dy == dps[0].cols());
  FFT2 fft(prb_dx, prb_dy, FFTW_FORWARD);
  FFT2 ifft(prb_dx, prb_dy, FFTW_BACKWARD);

  std::vector<double> errors(n_iters);
  double error_per_iter;
  int x_pos, y_pos;

  ArrayXcdRM psi(prb_dx, prb_dy);
  ArrayXcdRM psi_k(prb_dx, prb_dy);
  ArrayXcdRM psi_p(prb_dx, prb_dy);
  ArrayXcdRM d_psi(prb_dx, prb_dy);
  ArrayXcdRM lil_obj(prb_dx, prb_dy);

  for (int iter = 1; iter <= n_iters; ++iter) {
    error_per_iter = 0;
    for (int k = 0; k < n_dps; ++k) {
      x_pos = scan_pos(k, 0);
      y_pos = scan_pos(k, 1);

      lil_obj = obj(eig::seqN(x_pos, prb_dx), eig::seqN(y_pos, prb_dy)).array();

      // exit wave and its Fourier transform
      psi = lil_obj * prb;
      fft.compute(psi, psi_k);

      // replace modulus with (amplitude of..) diffraction pattern
      ifft.compute(dps[k] * psi_k / psi_k.abs(), psi_p);
      d_psi = psi_p - psi;

      // update obj, prb, error

      obj(eig::seqN(x_pos, prb_dx), eig::seqN(y_pos, prb_dy)) = lil_obj +
        obj_step * d_psi * prb.conjugate() / prb.abs().pow(2).maxCoeff();

      prb = prb + prb_step *
        d_psi * lil_obj.conjugate() / lil_obj.abs().pow(2).maxCoeff();

      error_per_iter += std::pow(d_psi.abs().sum()/(prb_dx*prb_dy), 2);

    }
    errors[iter - 1] = error_per_iter;
  }

  return errors;
}
