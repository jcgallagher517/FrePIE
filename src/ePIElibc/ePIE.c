#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <fftw3.h>

void compute_fft(size_t size, double *in_real, double *in_imag, double *out_real, double *out_imag) {

  // initialize inputs and outputs as complex numbers
  fftw_complex *in, *out;
  in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*size);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex)*size);
  for (int i = 0; i < size; ++i) {
    in[i] = in_real[i] + I*in_imag[i];
    printf("%f + i%f\n", creal(in[i]), cimag(in[i]));
  }

  printf("\n\n");

  // execute one-dimensional FFT on input
  fftw_plan p = fftw_plan_dft_1d(size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);

  for (int i = 0; i < size; ++i) {
    out_real[i] = creal(out[i]);
    out_imag[i] = cimag(out[i]);
    printf("%f + i%f\n", out_real[i], out_imag[i]);
  }

  fftw_destroy_plan(p);
  fftw_free(in); fftw_free(out);
}

