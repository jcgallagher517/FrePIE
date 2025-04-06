#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <fftw3.h>

void fft(size_t size, double complex *signal, double complex *output) {
  fftw_plan p = fftw_plan_dft_1d(size, signal, output, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_execute(p);
  fftw_destroy_plan(p);
}

// remove main when shipping as python library
int main() {

  size_t N = 16;
  double complex signal[N], output[N];

  // initialize data
  for (int i = 0; i < N; ++i) {
    signal[i] = cos(i) + I*sin(i);
  }

  // apply fft
  fft(N, signal, output);

  // print input and output
  for (int i = 0; i < N; ++i) {
    printf("%.2f exp(i%.2f)\n", cabs(output[i]), carg(output[i]));
  }

  return 0;
}
