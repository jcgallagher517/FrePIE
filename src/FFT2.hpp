#ifndef FFT2_HPP
#define FFT2_HPP

/* class for doing 2D fast-fourier-transforms of complex-valued arrays
 */

#include <Eigen/Dense>
#include <fftw3.h>

namespace eig = Eigen;
using ArrayXcdRM = eig::Array<std::complex<double>, eig::Dynamic, eig::Dynamic, eig::RowMajor>;

class FFT2 {
public:

  /* constructor and destructor
     initializes FFT plan to apply for rows x cols array 
     direction: FFTW_FORWARD or FFTW_BACKWARD
   */
  FFT2(int rows, int cols, int direction);
  ~FFT2();

  void compute(const ArrayXcdRM& input, ArrayXcdRM& output);

  ArrayXcdRM fftshift(const ArrayXcdRM& input);
  ArrayXcdRM ifftshift(const ArrayXcdRM& input);

private:
  
  int rows_, cols_;
  int direction_;
  ArrayXcdRM in_, out_;
  fftw_plan plan_;

  ArrayXcdRM circshift(const ArrayXcdRM& input, int row_shift, int col_shift);

};


#endif // FFT2_HPP
