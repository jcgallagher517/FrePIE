#include <complex>
#include <assert.h>
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

void FFT2::compute(const MatXcdRM& input, MatXcdRM& output) {
  in_ = input;
  fftw_execute(plan_);
  output = out_;
  
  if (direction_ == FFTW_BACKWARD) {
    output /= rows_ * cols_;
  }
}

void FFT2::circshift(const MatXcdRM& input, MatXcdRM& output, int row_shift, int col_shift) {
  int rows = input.rows();
  int cols = input.cols();
  assert(rows == output.rows() && cols == output.cols());

  // credit to this stackoverflow answer
  // https://stackoverflow.com/questions/5915125/fftshift-ifftshift-c-c-source-code
  for (int i = 0; i < rows; ++i) {
    int ii = (i + row_shift) % rows;
    for (int j = 0; j < cols; ++j) {
      int jj = (j + col_shift) % cols;
      output(ii, jj) = input(i, j);
    }
  }
}

void FFT2::fftshift(const MatXcdRM& input, MatXcdRM& output) {
  FFT2::circshift(input, output, input.rows()/2, input.cols()/2);
}

void FFT2::ifftshift(const MatXcdRM& input, MatXcdRM& output) {
  FFT2::circshift(input, output, (input.rows() + 1)/2, (input.cols() + 1)/2);
}
