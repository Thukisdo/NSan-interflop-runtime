// RUN: %clangxx_nsan -O0 -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// RUN: %clangxx_nsan -O1 -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// RUN: %clangxx_nsan -O2 -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// This tests Kahan's Smooth Surprise:
// http://arith22.gforge.inria.fr/slides/06-gustafson.pdf
// log(|3(1–x)+1|)/80 + x2 + 1
//
// This implementation using floats consistently gives the wrong answer, and
// this cannot be caught by nsan, because the issue here is not the numerical
// instability of the computations (`SmoothSurprise` is stable), but the density
// of the floats.

#include <cmath>
#include <cstdio>
#include <cstdint>

#include <iostream>
#include <limits>

double SmoothSurprise(double x) {
  return log(abs(3 * (1 - x) + 1))/80.0 + x * x + 1;
}

int main() {
  double x_min = 0.0;
  double y_min = std::numeric_limits<double>::max();
  constexpr const double kStart = 0.8;
  constexpr const double kEnd = 2.0;
  constexpr const int kNumSteps = 500000;  // Half a million.
  for (int i = 0; i < kNumSteps; ++i) {
    const double x = kStart + (i * (kEnd - kStart)) / kNumSteps;
    const double y = SmoothSurprise(x);
    if (y < y_min) {
      x_min = x;
      y_min = y;
    }
  }
  printf("Minimum at x=%.8f (f(x)=%.8f)\n", x_min, y_min);
  return 0;
}
