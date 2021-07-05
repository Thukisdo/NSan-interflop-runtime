#pragma once

#include <cmath>
#include <limits>

namespace std {

// std::ostream does not natively support __float128  
std::ostream& operator<<(std::ostream &os, __float128 x) {
  return os << static_cast<double>(x);
}
} // namespace std

typedef float v2float __attribute__((vector_size(8)));
typedef float v4float __attribute__((vector_size(16)));
typedef float v8float __attribute__((vector_size(32)));
typedef double v2double __attribute__((vector_size(16)));
typedef double v4double __attribute__((vector_size(32)));
typedef double v8double __attribute__((vector_size(64)));

void __nsan_dump_stacktrace();

namespace utils {

void DumpStacktrace() {
  std::cout << "\n\nInterflop stacktrace : \n";
  __nsan_dump_stacktrace();
  std::cout << "\n\n";
}

template <typename T> T abs(T x) { return (x < 0) ? -x : x; }
template <typename T> bool isnan(T x) { return std::isnan(x); }
template <> bool isnan(__float128 x) {
  return std::isnan(static_cast<double>(x));
}

[[noreturn]] void unreachable(const char* str)
{
  if (str)
    std::cout << str << std::endl;
  std::cout << "Interflop unreachable called !\n";
  DumpStacktrace();
  exit(1);
}

} // namespace utils