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

  // fftw_init_threads();
  // fftw_plan_with_nthreads(2);

  plan_ = fftw_plan_dft_2d(rows_, cols_,
                           reinterpret_cast<fftw_complex*>(in_.data()),
                           reinterpret_cast<fftw_complex*>(out_.data()),
                           direction, FFTW_MEASURE);
}

FFT2::~FFT2() {
  fftw_destroy_plan(plan_);
}

void FFT2::compute(const ArrayXcdRM& input, ArrayXcdRM& output) {
  in_ = input;
  fftw_execute(plan_);
  output = out_;
  
  if (direction_ == FFTW_BACKWARD) {
    output /= rows_ * cols_;
  }

}

ArrayXcdRM FFT2::circshift(const ArrayXcdRM &input, int row_shift, int col_shift) {
  int rows = input.rows();
  int cols = input.cols();
  ArrayXcdRM output(rows, cols);
  assert(rows == output.rows() && cols == output.cols());

  // credit to this stackoverflow answer
  // https://stackoverflow.com/questions/5915125/fftshift-ifftshift-c-c-source-code
  for (int r = 0; r < rows; ++r) {
    int rr = (r + row_shift) % rows;
    for (int c = 0; c < cols; ++c) {
      int cc = (c + col_shift) % cols;
      output(rr, cc) = input(r, c);
    }
  }
  return output;
}

ArrayXcdRM FFT2::fftshift(const ArrayXcdRM& input) {
  return FFT2::circshift(input, input.rows()/2, input.cols()/2);
}

ArrayXcdRM FFT2::ifftshift(const ArrayXcdRM& input) {
  return FFT2::circshift(input, (input.rows() + 1)/2, (input.cols() + 1)/2);
}
