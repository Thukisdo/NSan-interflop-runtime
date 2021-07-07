#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>

// The stl does not provide a typedef for uint128
typedef unsigned __int128 uint128_t;

#ifndef MCASYNC_DEFINITION

#ifndef MCASYNC_DECLARATION
#define MCASYNC_DECLARATION
namespace mcasync {

template <typename> class MCASyncRuntime;
template <typename, FCmpOpcode> class ShadowFCmp;

class MSASyncRuntimeInfo {
public:
  struct shadow128_t {
    float val[3];
    uint32_t padding[1];

    // helper methods for printing / acessing one of the shadow float
    friend std::ostream &operator<<(std::ostream &os, shadow128_t const &s);
    inline float operator[](size_t const index) const {
      assert(index < 3);
      return val[index];
    }
  };

  struct shadow256_t {
    double val[3];
    uint64_t padding[1];

    // helper methods for printing / acessing one of the shadow double
    friend std::ostream &operator<<(std::ostream &os, shadow256_t const &s);
    inline double operator[](size_t const index) const {
      assert(index < 3);
      return val[index];
    }
  };

  template <typename FPType> using Runtime = MCASyncRuntime<FPType>;
};
} // namespace mcasync
#endif

#elif defined(MCASYNC_DEFINITION)

#pragma once
#include <cmath>
#include <iomanip>

#include "Flags.hpp"

#include "Shadow.hpp"
#include "Utils.hpp"

namespace mcasync {

// we print the mean value + every element of s
std::ostream &operator<<(std::ostream &os, shadow128_t const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cout << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, shadow256_t const &s) {
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
  static const Float64 oneF64 = 1.0;
  static const Float64 eps_F32 =
      std::nextafter((double)std::nextafter(0.0f, 1.0f), 0.0);

  uint64_t RandomBits = utils::rand<uint64_t>();
  // subnormals are rounded with float-arithmetic for uniform stoch perturbation
  // (Magic)
  if (utils::abs(x) < std::numeric_limits<float>::min()) {
    Float64 Res(oneF64.i64 | (RandomBits >> 12));
    Res.f64 -= 1.5;
    return std::roundf(x + eps_F32.f64 * Res.f64);
  }

  Float64 ExtendedFP(x);
  // arithmetic bitshift and |1 to create a random integer that is in (-u/2,u/2)
  // always set last random bit to 1 to avoid the creation of -u/2
  ExtendedFP.i64 += (RandomBits >> 35) | 1;
  return std::roundf(ExtendedFP.f64);
}

// Helper struct for type puning f128 -> ui128
struct Float128 {
  Float128(__float128 f) : f128(f) {}
  Float128(__uint128_t i) : i128(i) {}
  Float128(double f)
      : f128(f) {} // Strangely, the compiler doesn't know wether to implicitly
                   // convert a double to f128 or i128, hence we need a double ctor
                   // to avoit the cast altogether

  union {
    __float128 f128;
    __uint128_t i128;
  };
};

// Adapted from a Julia rounding code
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
double StochasticRound(double x) {
  static const Float128 oneF128 = 1.0;
  static const Float128 eps_F64 =
      std::nextafter((double)std::nextafter(0.0f, 1.0f), 0.0);

  uint128_t RandomBits = utils::rand<uint64_t>();
  RandomBits |= ((uint128_t)utils::rand<uint64_t>()) << 64;
  // subnormals are round with float-arithmetic for uniform stoch perturbation
  // (Magic)
  if (utils::abs(x) < std::numeric_limits<double>::min()) {
    Float128 Res(oneF128.i128 | (RandomBits >> 12));
    Res.f128 -= 1.5;
    return roundf(x + eps_F64.f128 * Res.f128);
  }
  Float128 ExtendedFP(x);
  // arithmetic bitshift and |1 to create a random integer that is in (-u/2,u/2)
  // always set last random bit to 1 to avoid the creating of -u/2
  ExtendedFP.i128 = (ExtendedFP.i128 + (RandomBits >> 35)) | 1;
  return roundf(ExtendedFP.f128);
}


// Double precision runtime using c++ templates
// Scalar and vectors are treated in the same methods
template <typename FPType> class MCASyncRuntime {
public:
  using ScalarVT = typename shadow_type<FPType>::scalar_type;
  using ShadowType = typename shadow_type<FPType>::type;
  using ShadowPtr = ShadowType **;
  static constexpr size_t VectorSize = shadow_type<FPType>::VectorSize;

  // The class should be used statically
  MCASyncRuntime() = delete;
  MCASyncRuntime(MCASyncRuntime const &) = delete;

  // Binary operator overload
  static FPType Add(FPType a, ShadowPtr const sa, FPType b, ShadowPtr const sb,
                    ShadowPtr res) {
    for (int I = 0; I < VectorSize; I++) {
      res[I]->val[0] = StochasticRound(sa[I]->val[0] + sb[I]->val[0]);
      res[I]->val[1] = StochasticRound(sa[I]->val[1] + sb[I]->val[1]);
      res[I]->val[2] = StochasticRound(sa[I]->val[2] + sb[I]->val[2]);
    }
    return a + b;
  }

  static FPType Sub(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                    ShadowPtr res) {
    for (int I = 0; I < VectorSize; I++) {
      res[I]->val[0] = StochasticRound(sa[I]->val[0] - sb[I]->val[0]);
      res[I]->val[1] = StochasticRound(sa[I]->val[1] - sb[I]->val[1]);
      res[I]->val[2] = StochasticRound(sa[I]->val[2] - sb[I]->val[2]);
    }
    return a - b;
  }

  static FPType Mul(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                    ShadowPtr res) {
    for (int I = 0; I < VectorSize; I++) {
      res[I]->val[0] = StochasticRound(sa[I]->val[0] * sb[I]->val[0]);
      res[I]->val[1] = StochasticRound(sa[I]->val[1] * sb[I]->val[1]);
      res[I]->val[2] = StochasticRound(sa[I]->val[2] * sb[I]->val[2]);
    }
    return a * b;
  }

  static FPType Div(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                    ShadowPtr res) {

    for (int I = 0; I < VectorSize; I++) {
      res[I]->val[0] = StochasticRound(sa[I]->val[0] / sb[I]->val[0]);
      res[I]->val[1] = StochasticRound(sa[I]->val[1] / sb[I]->val[1]);
      res[I]->val[2] = StochasticRound(sa[I]->val[2] / sb[I]->val[2]);
    }
    return a / b;
  }

  template <FCmpOpcode Opcode> static bool FCmp(ShadowPtr sa, ShadowPtr sb) {

    bool res = true;

    for (int I = 0; res && (I < VectorSize); I++) {

      double LeftOp = (sa[I]->val[0] + sa[I]->val[1] + sa[I]->val[2]) / 3;
      double RightOp = (sb[I]->val[0] + sb[I]->val[1] + sb[I]->val[2]) / 3;

      // Handle unordered comparisons
      if (Opcode < UnorderedFCmp &&
          (utils::isnan(LeftOp) || utils::isnan(RightOp)))
        return true;

      // Handle (ordered) comparisons
      if constexpr (Opcode == FCmp_oeq || Opcode == FCmp_ueq)
        res = res && (LeftOp == RightOp);
      else if constexpr (Opcode == FCmp_one || Opcode == FCmp_one)
        res = res && (LeftOp != RightOp);
      else if constexpr (Opcode == FCmp_ogt || Opcode == FCmp_ogt)
        res = res && (LeftOp > RightOp);
      else if constexpr (Opcode == FCmp_olt || Opcode == FCmp_olt)
        res = res && (LeftOp < RightOp);
      else
        utils::unreachable("Unknown Predicate");
    }
    return res;
  }

  template <FCmpOpcode Opcode>
  static bool CheckFCmp(bool c, FPType a, FPType b, ShadowPtr sa,
                        ShadowPtr sb) {
    bool res = FCmp<Opcode>(sa, sb);
    // We expect both comparison to be equal, else we print an error
    if (not RuntimeFlags::DisableWarning && c != res)
      FCmpCheckFail(a, sa, b, sb);
    // We return the shadow comparison result to be able to correctly branch
    return res;
  }

  // Handles both up and downcast
  // FIXME : add type checking for the destination shadow
  template <typename DestVT>
  static void Cast(FPType a, ShadowPtr sa, DestVT **res) {
    for (int I = 0; I < VectorSize; I++) {
      res[I]->val[0] = sa[I]->val[0];
      res[I]->val[1] = sa[I]->val[1];
      res[I]->val[2] = sa[I]->val[2];
    }
  }

  static FPType Neg(FPType a, ShadowPtr sa, ShadowPtr res) {
    for (int I = 0; I < VectorSize; I++) {
      res[I]->val[0] = -sa[I]->val[0];
      res[I]->val[1] = -sa[I]->val[1];
      res[I]->val[2] = -sa[I]->val[2];
    }
    return -a;
  }

  static void MakeShadow(FPType a, ShadowPtr res) {
    for (int I = 0; I < VectorSize; I++) {
      // We need a constexpr if to prevent the compiler from evaluating a[I] if
      // its a scalar
      if constexpr (VectorSize > 1) {
        res[I]->val[0] = a[I];
        res[I]->val[1] = a[I];
        res[I]->val[2] = a[I];
      } else {
        res[0]->val[0] = a;
        res[0]->val[1] = a;
        res[0]->val[2] = a;
      }
    }
  }

  static int Check(FPType a, ShadowPtr sa) {

    // We unvectorize the check
    if constexpr (VectorSize > 1) {
      bool Res = 0;
      for (int I = 0; I < VectorSize; I++) {
        Res = Res || MCASyncRuntime<ScalarVT>::Check(a[I], &sa[I]);
      }
      return Res;
    }

    double Mean = (sa[0]->val[0] + sa[0]->val[1] + sa[0]->val[2]) / 3;

    double Variance = 0;
    for (int I = 0; I < 3; I++) {
      Variance += pow(Mean - sa[0]->val[0], 2);
    }
    Variance /= 3;
    double Exp = std::log10(utils::abs(Variance / Mean));

    if (Exp >= 5.0) {
      if (not RuntimeFlags::DisableWarning) {
        std::cout << "\033[1;31m";
        std::cout << "[MCASync] Inconsistent shadow result :" << std::endl;
        std::cout << "\tNative Value: " << std::setprecision(10) << a
                  << std::endl;
        std::cout << "\tShadow Value: \n\t  " << *sa[0] << std::endl;
        std::cout << "E : " << Exp << " Variance: " << Variance << std::endl;
        utils::DumpStacktrace();
        std::cout << "\033[0m";
      }
      if (RuntimeFlags::ExitOnError)
        exit(1);
      return 1;
    }
    return 0;
  }

private:
  static void FCmpCheckFail(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb) {

    std::cout << "\033[1;31m";
    std::cout
        << "[MCASync] Floating-point comparison results depend on precision"
        << std::endl;
    std::cout << "\tValue a: " << std::setprecision(10) << a << " b: " << b
              << std::endl;
    std::cout << "Shadow a:\n";
    for (int I = 0; I < VectorSize; I++) {
      std::cout << "\t" << *sa[I] << "\n";
    }
    std::cout << "Shadow b:\n";
    for (int I = 0; I < VectorSize; I++) {
      std::cout << "\t" << *sb[I] << "\n";
    }
    utils::DumpStacktrace();
    std::cout << "\033[0m";
    if (RuntimeFlags::ExitOnError)
      exit(1);
  }
};
} // namespace mcasync
#endif