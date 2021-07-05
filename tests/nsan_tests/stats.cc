// RUN: %clangxx_nsan -O0 -mllvm -nsan-shadow-type-mapping=dqq -g %s -o %t && NSAN_OPTIONS=halt_on_error=0,disable_warnings=1,enable_check_stats=1,enable_warning_stats=1,print_stats_on_exit=1 %run %t >%t.out 2>&1
// Checked separately because the order is not deterministic.
// RUN: FileCheck %s --check-prefix=WARNING < %t.out
// RUN: FileCheck %s --check-prefix=NOWARNING < %t.out

// This tests the "stats" mode of nsan.
// In this test:
//  - we do not stop the application on error (halt_on_error=0),
//  - we disable real-time printing of warnings (disable_warnings=1),
//  - we enable stats collection (enable_{check,warning}_stats=1),
//  - we print stats when exiting the application (print_stats_on_exit=1).
// We then check that the application correctly collected stats about the checks
// that were done and where those checks resulted in warnings.

#include "helpers.h"

#include <cstdio>

int main() {
  double d;
  CreateInconsistency(&d);
  DoNotOptimize(d);
  printf("%.16f\n", d);
  // WARNING: warned 1 times out of {{[0-9]*}} argument checks (max relative error:
  // {{.*}}%) at WARNING-NEXT:#0{{.*}} in main{{.*}}stats.cc:[[@LINE-2]]
  d = 42;
  printf("%.16f\n", d);
  // NOWARNING: warned 0 times out of {{[0-9]*}} argument checks at
  // NOWARNING-NEXT:#0{{.*}} in main{{.*}}stats.cc:[[@LINE-2]]
  return 0;
}
