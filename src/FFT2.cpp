#include <complex>
#include <Eigen/Dense>
#include <fftw3.h>

#include "FFT2.hpp"

// not sure if this stuff is already included by using FFT2.hpp
// namespace eig = Eigen;
// using MatXcdRM = eig::Matrix<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;

FFT2::FFT2(int rows, int cols, int direction)
    : rows_(rows),
      cols_(cols),
      direction_(direction),
      in_(rows, cols),
      out_(rows, cols)
{
  plan_ = fftw_plan_dft_2d(rows_, cols_,
                           reinterpret_cast<fftw_complex*>(in_.data()),
                           reinterpret_cast<fftw_complex*>(out_.data()),
                           direction, FFTW_MEASURE);
}

FFT2::~FFT2() {
  fftw_destroy_plan(plan_);
}

void FFT2::compute(const MatXcdRM& input, MatXcdRM output) {
  in_ = input;
  fftw_execute(plan_);
  output = out_;
  
  if (direction_ == FFTW_BACKWARD) {
    output /= rows_ * cols_;
  }
}
