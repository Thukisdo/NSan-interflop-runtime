// RUN: %clangxx_nsan -O0 -DFN=Unstable -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=UNSTABLE < %t.out

// RUN: %clangxx_nsan -O2 -DFN=Unstable -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=UNSTABLE < %t.out

// RUN: %clangxx_nsan -O0 -DFN=StableRel -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// RUN: %clangxx_nsan -O2 -DFN=StableRel -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// RUN: %clangxx_nsan -O0 -DFN=StableEq -mllvm -nsan-truncate-fcmp-eq=true -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// RUN: %clangxx_nsan -O2 -DFN=StableEq -mllvm -nsan-truncate-fcmp-eq=true -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 %run %t

// RUN: %clangxx_nsan -O0 -DFN=StableEq -mllvm -nsan-truncate-fcmp-eq=false -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=STABLEEQ-NOTRUNCATE < %t.out

// RUN: %clangxx_nsan -O2 -DFN=StableEq -mllvm -nsan-truncate-fcmp-eq=false -g %s -o %t && NSAN_OPTIONS=halt_on_error=1 not %run %t >%t.out 2>&1
// RUN: FileCheck %s --check-prefix=STABLEEQ-NOTRUNCATE < %t.out

// http://cadna.lip6.fr/Examples_Dir/ex5.php
// This program computes a root of the polynomial
//   f(x) = 1.47*x**3 + 1.19*x**2 - 1.83*x + 0.45
// using Newton's method.
// The sequence is initialized by x = 0.5.
// The iterative algorithm `x(n+1) = x(n) - f(x(n))/f'(x(n))` is stopped by the
// criterion |x(n)-x(n-1)|<=1.0e-12.
//
// The first algorithm is inherently unstable, this checks that nsan detects the
// issue with the unstable code and does not trigger on the stabilized version.

#include <cmath>
#include <cstdio>

constexpr const double kEpsilon = 1e-12;
constexpr const double kNMax = 100;

#define FN Unstable

// The unstable version.
// Adapted from Fortran: http://cadna.lip6.fr/Examples_Dir/source/ex5.f
__attribute__((noinline)) // Prevent constant folding.
void
Unstable(double y) {
  double x;
  int i;
  for (i = 1; i < kNMax; ++i) {
    x = y;
    y = x - (1.47 * x * x * x + 1.19 * x * x - 1.83 * x + 0.45) /
                (4.41 * x * x + 2.38 * x - 1.83);
    if (fabs(x - y) < kEpsilon)
      break;
    // UNSTABLE: #0{{.*}}in Unstable{{.*}}cadna_ex5.cc:[[@LINE-1]]
  }

  printf("x(%i) = %g\n", i - 1, x);
  printf("x(%i) = %g\n", i, y);
}

// The stabilized version, where the termination criterion is an equality
// comparison. The equality is considered unstable or not by nsan depending on
// the value of --nsan-truncate-fcmp-eq.
// Adapted from Fortran: http://cadna.lip6.fr/Examples_Dir/source/ex5_cad_opt.f
__attribute__((noinline)) // Prevent constant folding.
void
StableEq(double y) {
  double x;
  int i;
  for (i = 1; i < kNMax; ++i) {
    x = y;
    y = ((4.2 * x + 3.5) * x + 1.5) / (6.3 * x + 6.1);
    if (x == y)
      break;
    // STABLEEQ-NOTRUNCATE: #0{{.*}}in StableEq{{.*}}cadna_ex5.cc:[[@LINE-1]]
  }

  printf("x(%i) = %g\n", i - 1, x);
  printf("x(%i) = %g\n", i, y);
}

// The stabilized version, where the termination criterion is a relative
// comparison. This is a more stable fix of `Unstable`.
__attribute__((noinline)) // Prevent constant folding.
void
StableRel(double y) {
  double x;
  int i;
  for (i = 1; i < kNMax; ++i) {
    x = y;
    y = ((4.2 * x + 3.5) * x + 1.5) / (6.3 * x + 6.1);
    if (fabs(x - y) < kEpsilon)
      break;
  }

  printf("x(%i) = %g\n", i - 1, x);
  printf("x(%i) = %g\n", i, y);
}

int main() {
  FN(0.5);
  return 0;
}
