#include <iostream>
#include <limits>

// We declare the shadow type used by this backend, and the runtime class
#ifndef RUNTIME_DEFINITION

#ifndef DOUBLEPREC_DECLARATION
#define DOUBLEPREC_DECLARATION
namespace doubleprec {

template <typename> class DoublePrecRuntime;
template <typename, FCmpOpcode> class ShadowFCmp;

class DoublePrecRuntimeInfo {
public:
  struct shadow128_t {
    double val;
    uint64_t padding[1];
  };

  struct shadow256_t {
    __float128 val;
    uint64_t padding[2];
  };

  template <typename FPType> using Runtime = DoublePrecRuntime<FPType>;
};
} // namespace doubleprec
#endif

// The actual backend should only be defined once in Interface.cpp, the rest of
// the library should only rely on the definitions above
#elif defined(RUNTIME_DEFINITION)

#pragma once
#include <cmath>
#include <iomanip>

#include "Flags.hpp"

#include "Shadow.hpp"
#include "Utils.hpp"

namespace doubleprec {

// Double precision runtime using c++ templates
// Scalar and vectors are treated in the same methods
template <typename FPType> class DoublePrecRuntime {
public:
  using ScalarVT = typename shadow_type<FPType>::scalar_type;
  using ShadowType = typename shadow_type<FPType>::type;
  using ShadowPtr = ShadowType **;
  static constexpr size_t VectorSize = shadow_type<FPType>::VectorSize;

  // The class should be used statically
  DoublePrecRuntime() = delete;
  DoublePrecRuntime(DoublePrecRuntime const &) = delete;

  // Binary operator overload
  static FPType Add(FPType a, ShadowPtr const sa, FPType b, ShadowPtr const sb,
                    ShadowPtr res) {
    for (int i = 0; i < VectorSize; i++)
      res[i]->val = sa[i]->val + sb[i]->val;
    return a + b;
  }

  static FPType Sub(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                    ShadowPtr res) {
    for (int i = 0; i < VectorSize; i++)
      res[i]->val = sa[i]->val - sb[i]->val;
    return a - b;
  }

  static FPType Mul(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                    ShadowPtr res) {
    for (int i = 0; i < VectorSize; i++)
      res[i]->val = sa[i]->val * sb[i]->val;
    return a * b;
  }

  static FPType Div(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                    ShadowPtr res) {

    for (int i = 0; i < VectorSize; i++)
      res[i]->val = sa[i]->val / sb[i]->val;
    return a / b;
  }

  template <FCmpOpcode Opcode> static bool FCmp(ShadowPtr sa, ShadowPtr sb) {

    bool res = true;

    for (int I = 0; res && (I < VectorSize); I++) {

      auto LeftOp = sa[I]->val;
      auto RightOp = sb[I]->val;

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
    for (int i = 0; i < VectorSize; i++) {
      res[i]->val = sa[i]->val;
    }
  }

  static FPType Neg(FPType a, ShadowPtr sa, ShadowPtr res) {
    for (int i = 0; i < VectorSize; i++) {
      res[i]->val = -sa[i]->val;
    }
    return -a;
  }

  static void MakeShadow(FPType a, ShadowPtr res) {
    for (int i = 0; i < VectorSize; i++) {
      // We need a constexpr if to prevent the compiler from evaluating a[i] if
      // its a scalar
      if constexpr (VectorSize > 1)
        res[i]->val = a[i];
      else
        res[0]->val = a;
    }
  }

  static int Check(FPType a, ShadowPtr sa) {

    // We unvectorize the check
    if constexpr (VectorSize > 1) {
      bool res = 0;
      for (int i = 0; i < VectorSize; i++) {
        res = res || DoublePrecRuntime<ScalarVT>::Check(a[i], &sa[i]);
      }
      return res;
    }

    double AbsoluteErrror = utils::abs(a - sa[0]->val);
    double RelativeError = utils::abs((AbsoluteErrror / sa[0]->val) * 100);
    if (RelativeError >= 0.0001) {
      if (not RuntimeFlags::DisableWarning) {
        std::cout << "\033[1;31m";
        std::cout << "Inconsistent shadow result :" << std::endl;
        std::cout << "\tNative Value " << a << std::endl;
        std::cout << "\tShadow Value " << sa[0]->val << std::endl;
        std::cout << "\tRelative error " << RelativeError << std::endl;
        std::cout << "\tAbsolute error " << AbsoluteErrror << std::endl;
        std::cout << "\033[0m";
        utils::DumpStacktrace();
      }
      if (RuntimeFlags::ExitOnError)
        exit(1);
      return 1;
    }
    return 0;
  }

private:
  static void FCmpCheckFail(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb) {
    std::cout << "floating-point comparison results depend on precision"
              << std::endl;
    std::cout << "\tValue a: " << a << " b: " << b << std::endl;
    std::cout << "Shadow a: { ";
    for (int i = 0; i < VectorSize; i++) {
      std::cout << sa[i]->val << " ";
    }
    std::cout << "} \nShadow b: { " << std::endl;
    for (int i = 0; i < VectorSize; i++) {
      std::cout << sb[i]->val << " ";
    }
    std::cout << "}" << std::endl;
    utils::DumpStacktrace();
    if (RuntimeFlags::ExitOnError)
      exit(1);
  }
};
} // namespace doubleprec
#endif