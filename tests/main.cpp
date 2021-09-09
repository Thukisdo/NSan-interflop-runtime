#include <float.h>
#include <random>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
struct complex {
  double real, imag;
};

double __attribute__((noinline)) power2(double x) { return x * x; }

double __attribute__((noinline)) compute(complex *a, complex *b) {
  double sum = 0;

  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      sum += a->real * b->real + a->imag * b->imag;
  return sum;
}

int main() {

  complex *a = (complex *)malloc(sizeof(complex));
  complex *b = (complex *)malloc(sizeof(complex));
  a->real = 1;
  b->real = 4;

  // No need for further initialization for the bug to appear

  double c = 0;

  c = compute(a, b);
  c = power2(c);
  free(a);
  free(b);
  printf("C: %lf\n", c);
  return c;
}