/**
 * @file Utils.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Defines a set of utility functions.
 * @version 0.7.0
 * @date 2021-07-20
 *
 */

#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <random>

namespace std {

// std::ostream does not natively support __float128
// Inline attribute is required to avoid multiple definitions
inline std::ostream &operator<<(std::ostream &os, __float128 x) {
  return os << static_cast<double>(x);
}
} // namespace std

namespace interflop {

typedef float v2float __attribute__((vector_size(8)));
typedef float v4float __attribute__((vector_size(16)));
typedef float v8float __attribute__((vector_size(32)));
typedef float v16float __attribute__((vector_size(64)));
typedef double v2double __attribute__((vector_size(16)));
typedef double v4double __attribute__((vector_size(32)));
typedef double v8double __attribute__((vector_size(64)));
// FIXME : We should handle long double

namespace utils {

enum FloatType {
  kFloat,
  kV2Float,
  kV4Float,
  kV8Float,
  kV16Float,
  kDouble,
  kV2Double,
  kV4Double,
  kV8Double,
  kNumFloatType
};

// Print the stack if debugging symbols are available
// NSan's runtime is required to be linked in.
void DumpStacktrace() noexcept;

// Save the current stacktrace and returns the save ID
uint32_t SaveStackTrace() noexcept;

// Print the stacktrace of given ID
void PrintStackTrace(uint32_t StackId) noexcept;

// Raise an error and terminate the program
[[noreturn]] void unreachable(const char *str) noexcept;
[[noreturn]] void exit(int status) noexcept;

// Return an integer between [LowerBound; Upperbound] included
// By default between [T::min; T::max]
// FIXME: This is not thread safe and quite slow
template <typename T>
T rand(const T LowerBound = std::numeric_limits<T>::min(),
       const T UpperBound = std::numeric_limits<T>::max()) {
  // We dont need the same distribution for int and float
  using uniform_distribution =
      typename std::conditional<std::is_integral<T>::value,
                                std::uniform_int_distribution<T>,
                                std::uniform_real_distribution<T>>::type;
  // Implementation dependant, generally quite heavy
  static std::default_random_engine RandomGenerator(time(nullptr));

  // Lightweight object
  uniform_distribution distribution(LowerBound, UpperBound);
  return distribution(RandomGenerator);
}

// std::abs and std::isnan do not natively support __float128
template <typename T> T abs(T x) { return (x < 0) ? -x : x; }

// Required to check for unordered comparisons
template <typename T> bool isnan(T x) { return std::isnan(x); }

template <> inline bool isnan(__float128 x) {
  return std::isnan(static_cast<double>(x));
}






} // namespace utils
} // namespace interflop