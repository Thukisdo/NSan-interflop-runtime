// RUN: %clangxx_nsan -O2 -g -DFN=WcsDup %s -o %t && NSAN_OPTIONS=halt_on_error=0,resume_after_warning=false %run %t >%t.out 2>&1
// RUN: FileCheck --check-prefix=WCSDUP %s < %t.out

// RUN: %clangxx_nsan -O2 -g -DFN=WcpCpy %s -o %t && NSAN_OPTIONS=halt_on_error=0,resume_after_warning=false %run %t >%t.out 2>&1
// RUN: FileCheck --check-prefix=WCPCPY %s < %t.out

// RUN: %clangxx_nsan -O2 -g -DFN=WcsCpy %s -o %t && NSAN_OPTIONS=halt_on_error=0,resume_after_warning=false %run %t >%t.out 2>&1
// RUN: FileCheck --check-prefix=WCSCPY %s < %t.out

// RUN: %clangxx_nsan -O2 -g -DFN=WcsCat %s -o %t && NSAN_OPTIONS=halt_on_error=0,resume_after_warning=false %run %t >%t.out 2>&1
// RUN: FileCheck --check-prefix=WCSCAT %s < %t.out

// This test case checks libc wide string operations interception.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>

#include "helpers.h"

extern "C" void __nsan_dump_shadow_mem(const char *addr, size_t size_bytes,
                                       size_t bytes_per_line, size_t reserved);

void WcsDup(wchar_t *const s) {
  wchar_t *const dup = wcsdup(s);
  __nsan_dump_shadow_mem(reinterpret_cast<const char *>(dup), 8, 8, 0);
  free(dup);
  // WCSDUP: WcsDup
  // WCSDUP-NEXT: d0 d1 d2 d3 d4 d5 d6 d7
  // WCSDUP-NEXT: d0 d1 d2 d3 __ __ __ __
}

void WcpCpy(wchar_t *const s) {
  wchar_t buffer[] = L"abc\0";
  wcpcpy(buffer, s);
  __nsan_dump_shadow_mem(reinterpret_cast<const char *>(buffer), sizeof(buffer),
                         sizeof(buffer), 0);
  // WCPCPY: WcpCpy
  // WCPCPY-NEXT: d0 d1 d2 d3 d4 d5 d6 d7
  // WCPCPY-NEXT: d0 d1 d2 d3 __ __ __ __
}

void WcsCpy(wchar_t *const s) {
  wchar_t buffer[] = L"abc\0";
  wcscpy(buffer, s);
  __nsan_dump_shadow_mem(reinterpret_cast<const char *>(buffer), sizeof(buffer),
                         sizeof(buffer), 0);
  // WCSCPY: WcsCpy
  // WCSCPY-NEXT: d0 d1 d2 d3 d4 d5 d6 d7
  // WCSCPY-NEXT: d0 d1 d2 d3 __ __ __ __
}

void WcsCat(wchar_t *const s) {
  wchar_t buffer[] = L"a\0    ";
  wcscat(buffer, s);
  __nsan_dump_shadow_mem(reinterpret_cast<const char *>(buffer), sizeof(buffer),
                         sizeof(buffer), 0);
  // WCSCAT: WcsCat
  // WCSCAT-NEXT: d0 d1 d2 d3 d4 d5 d6 d7
  // WCSCAT-NEXT: __ __ __ __ d0 d1 d2 d3 __ __ __ __
}

int main() {
  // This has binary representation 0x0000000080402010, which in memory
  // (little-endian) is {0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x00}.
  double f = 1.0630742122880717462525516679E-314;
  DoNotOptimize(f);
  wchar_t buffer[sizeof(double) / sizeof(wchar_t)];
  memcpy(buffer, &f, sizeof(double));
  static_assert(sizeof(wchar_t) == 4, "not implemented");
  printf("{0x%x, 0x%x}\n", buffer[0], buffer[1]);
#define str(s) #s
#define xstr(s) str(s)
  puts(xstr(FN));
  __nsan_dump_shadow_mem(reinterpret_cast<const char *>(buffer), sizeof(double),
                         sizeof(double), 0);
  FN(buffer);
  return 0;
}
