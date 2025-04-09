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

void ePIE(fftw_complex *obj, fftw_complex *prb,
          double *dps, double *scan_pos,
          double obj_step, double prb_step, int n_iters,
          int obj_dx, int obj_dy, int prb_dx, int prb_dy, int n_pats,
          double *error_arr) {

  int error_per_iter;

  for (int iter = 1; iter <= n_iters; ++iter) {

    

  }
}
