// RUN: %clangxx_nsan -O2 -g -DIMPL=OpEq %s -o %t

// RUN: rm -f %t.supp
// RUN: touch %t.supp
// RUN: NSAN_OPTIONS="halt_on_error=0,resume_after_warning=false,suppressions='%t.supp'" %run %t 2>&1 | FileCheck %s --check-prefixes=NOSUPP

// RUN: echo "consistency:*main*" > %t.supp
// RUN: NSAN_OPTIONS="halt_on_error=0,resume_after_warning=false,suppressions='%t.supp'" %run %t 2>&1 | FileCheck %s --check-prefixes=SUPP

// This tests sanitizer suppressions, i.e. warning silencing.

#include "helpers.h"

#include <cstdio>

int main() {
  double d;
  CreateInconsistency(&d);
  // NOSUPP: #1{{.*}}[[@LINE-1]]
  // SUPP-NOT: #1{{.*}}[[@LINE-2]]
  DoNotOptimize(d);
  printf("%.16f\n", d);
  // NOSUPP: #0{{.*}}[[@LINE-1]]
  // SUPP-NOT: #0[[@LINE-2]]
  return 0;
}
