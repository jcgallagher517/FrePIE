#ifndef FFT2_HPP
#define FFT2_HPP

/* class for doing 2D fast-fourier-transforms of complex-valued matrices
 */

#include <Eigen/Dense>
#include <fftw3.h>

namespace eig = Eigen;
using MatXcdRM = eig::Matrix<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;

class FFT2 {
public:

  /* constructor and destructor
     initializes FFT plan to apply for rows x cols matrix
     direction: FFTW_FORWARD or FFTW_BACKWARD
   */
  FFT2(int rows, int cols, int direction);
  ~FFT2();

  void compute(const MatXcdRM& input, MatXcdRM& output);

  void fftshift(const MatXcdRM& input, MatXcdRM& output);
  void ifftshift(const MatXcdRM& input, MatXcdRM& output);

private:
  
  int rows_, cols_;
  int direction_;
  MatXcdRM in_, out_;
  fftw_plan plan_;

  void circshift(const MatXcdRM& input, MatXcdRM& output, int row_shift, int col_shift);

};


#endif // FFT2_HPP
