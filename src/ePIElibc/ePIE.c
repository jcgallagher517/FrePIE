#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <fftw3.h>

/* trivial 1d fft example checks out
void compute_fft(size_t size, fftw_complex *input, fftw_complex *output) {
  fftw_plan p = fftw_plan_dft_1d(size, input, output, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);
  fftw_destroy_plan(p);
}
*/

