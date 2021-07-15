#include "backends/MCASync.hpp"

namespace interflop {
namespace mcasync {

std::ostream &operator<<(std::ostream &os, MCASyncShadow128 const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cout << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, MCASyncShadow256 const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cout << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

// Helper struct for type puning double -> ui64
struct Float64 {
  Float64(double f) : f64(f) {}
  Float64(uint64_t i) : i64(i) {}

  union {
    double f64;
    uint64_t i64;
  };
};

float StochasticRound(float x) {
  static Float64 oneF64 = 1.0;
  static Float64 eps_F32 =
      std::nextafter((double)std::nextafter(0.0f, 1.0f), 0.0);

  uint64_t RandomBits = utils::rand<uint64_t>();
  // subnormals are rounded with float-arithmetic for uniform stoch perturbation
  // (Magic)
  if (utils::abs(x) < std::numeric_limits<float>::min()) {
    Float64 Res(oneF64.i64 | (RandomBits >> 12));
    Res.f64 -= 1.5;
    return x + eps_F32.f64 * Res.f64;
  }

  Float64 ExtendedFP(x);
  // arithmetic bitshift and |1 to create a random integer that is in (-u/2,u/2)
  // always set last random bit to 1 to avoid the creation of -u/2
  ExtendedFP.i64 += (RandomBits >> 35) | 1;
  return ExtendedFP.f64;
}

// FIXME: Redundant code, should use template to only have one function
// Helper struct for type puning f128 -> ui128
struct Float128 {
  Float128(__float128 f) : f128(f) {}
  Float128(__uint128_t i) : i128(i) {}
  Float128(double f)
      : f128(f) {} // Strangely, the compiler doesn't know wether to implicitly
                   // cast a double to f128 or i128, hence we need a double
                   // ctor to avoid the cast altogether

  union {
    __float128 f128;
    __uint128_t i128;
  };
};

double StochasticRound(double x) {
  static Float128 oneF128 = 1.0;
  static Float128 eps_F64 =
      std::nextafter((double)std::nextafter(0.0, 1.0), 0.0);

  uint128_t RandomBits = utils::rand<uint64_t>();
  RandomBits |= ((uint128_t)utils::rand<uint64_t>()) << 64;
  // subnormals are round with float-arithmetic for uniform stoch perturbation
  // (Magic)
  if (utils::abs(x) < std::numeric_limits<double>::min()) {
    Float128 Res(oneF128.i128 | (RandomBits >> 12));
    Res.f128 -= 1.5;
    return x + eps_F64.f128 * Res.f128;
  }
  Float128 ExtendedFP(x);
  // arithmetic bitshift and |1 to create a random integer that is in (-u/2,u/2)
  // always set last random bit to 1 to avoid the creating of -u/2

  // FIXME : incorrect shift ?
  ExtendedFP.i128 = (ExtendedFP.i128 + (RandomBits >> 68)) | 1;
  return ExtendedFP.f128;
}

} // namespace mcasync
} // namespace interflop

/* 
distribution du stocastic / graph
500 sample + variance
tchebychev sampling 
*/