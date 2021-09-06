/**
 * @file Utils.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Defines a set of utility functions.
 * @version 0.5.1
 * @date 2021-08-31
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

namespace insane {

typedef float v2float __attribute__((vector_size(8)));
typedef float v4float __attribute__((vector_size(16)));
typedef float v8float __attribute__((vector_size(32)));
typedef float v16float __attribute__((vector_size(64)));
typedef double v2double __attribute__((vector_size(16)));
typedef double v4double __attribute__((vector_size(32)));
typedef double v8double __attribute__((vector_size(64)));
typedef long double v2ldouble __attribute__((vector_size(32)));
typedef long double v4ldouble __attribute__((vector_size(64)));

typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;

// Not defined in std::
// FIXME: Replace by std::numeric_limits<uint128_t> definition
constexpr uint128_t UINT128_MIN = 0;
constexpr uint128_t UINT128_MAX = ~((uint128_t)0);
constexpr int128_t INT128_MAX = UINT128_MAX >> 1;
constexpr int128_t INT128_MIN = -INT128_MAX - 1;
constexpr __float128 FLOAT128_INFINITY =
    std::numeric_limits<double>::infinity();

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
  klDouble,
  kV2lDouble,
  kV4lDouble,
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
// Thread safe
template <typename T> T rand() {
  // We dont need the same distribution for int and float
  using uniform_distribution =
      typename std::conditional<std::is_integral<T>::value,
                                std::uniform_int_distribution<T>,
                                std::uniform_real_distribution<T>>::type;
  // Implementation dependant, generally quite heavy
  // thread_local to prevent concurrency
  static thread_local std::default_random_engine RandomGenerator(time(nullptr));

  // Lightweight object
  uniform_distribution distribution(std::numeric_limits<T>::min(),
                                    std::numeric_limits<T>::max());
  return distribution(RandomGenerator);
}

// std:: does not natively support 128 bits types
// So we need to define our own rand function
// By combining multiple uint64_t
template <> inline __uint128_t rand<__uint128_t>() {

  __uint128_t Left64 = rand<uint64_t>();
  __uint128_t Right64 = rand<uint64_t>();

  return (Left64 << 64) | Right64;
}

template <> inline __int128_t rand<__int128_t>() {

  __int128_t Left64 = rand<int64_t>();
  __int128_t Right64 = rand<int64_t>();

  return (Left64 << 64) | Right64;
}

// std::abs and std::isnan do not natively support __float128
template <typename T> T abs(T x) { return (x < 0) ? -x : x; }

// Required to check for unordered comparisons
template <typename T> bool isnan(T x) { return std::isnan(x); }

template <> inline bool isnan(__float128 x) {
  return std::isnan(static_cast<double>(x));
}

size_t GetNSanShadowScale();

class AsciiColor {
public:
  static AsciiColor Red;
  static AsciiColor Yellow;
  static AsciiColor Blue;
  static AsciiColor Cyan;
  static AsciiColor Green;
  static AsciiColor Lime;
  static AsciiColor Magenta;
  static AsciiColor Gray;
  static AsciiColor Reset;

  // To be used with printf during initialization
  const char* str() const;
  friend std::ostream &operator<<(std::ostream &Os, const AsciiColor &Color);

private:
  AsciiColor(const char *str) noexcept : Str(str) {}
  const char *Str;
};

} // namespace utils
} // namespace insane