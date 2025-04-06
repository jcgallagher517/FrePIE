#include <stdio.h>

void test_fun(const double *indatav, size_t size, double *outdatav) {
  for (size_t i = 0; i < size; ++i) {
    outdatav[i] = indatav[i] * 2.0;
  }
}
