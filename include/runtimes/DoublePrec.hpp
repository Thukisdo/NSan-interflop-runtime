#pragma once
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "Flags.hpp"
#include "Shadow.hpp"
#include "Utils.hpp"

enum FCmpOpcode {
  kOrderedFCmp,
  kFCmp_oeq,
  kFCmp_one,
  kFCmp_ogt,
  kFCmp_olt,
  kUnorderedFCmp,
  kFCmp_ueq,
  kFCmp_une,
  kFCmp_ugt,
  kFCmp_ult
};

// Helper class to represent a predicate between two shadow value
// We need to handle the case where the compiler generated an unordered
// comparison
template <typename FPType, FCmpOpcode FCOp> class ShadowPredicate {
public:
  using ScalarVT = typename shadow_type<FPType>::scalar_type;
  using ShadowType = typename shadow_type<FPType>::type;
  using ShadowPtr = ShadowType **;
  static constexpr size_t VectorSize = shadow_type<FPType>::VectorSize;

  virtual bool Evaluate(ShadowType *LeftOp, ShadowType *RightOp) {
    // Handle unordered comparisons
    if (FCOp < kUnorderedFCmp && (utils::isnan<double>(LeftOp->val) ||
                                  utils::isnan<double>(RightOp->val)))
      return false;

    // Handle (ordered) comparisons
    if constexpr (FCOp == kFCmp_oeq || FCOp == kFCmp_ueq)
      return LeftOp->val == RightOp->val;
    if constexpr (FCOp == kFCmp_one || FCOp == kFCmp_one)
      return LeftOp->val != RightOp->val;
    if constexpr (FCOp == kFCmp_ogt || FCOp == kFCmp_ogt)
      return LeftOp->val > RightOp->val;
    if constexpr (FCOp == kFCmp_olt || FCOp == kFCmp_olt)
      return LeftOp->val < RightOp->val;
    utils::unreachable("Unknown Predicate");
  }
};

// Helper class to represent a comparison between two shadow value
// Handle the case where the shadows are vector and multiple scalar needs to be
// compared
template <typename FPType> class ShadowFCmp {
public:
  using ScalarVT = typename shadow_type<FPType>::scalar_type;
  using ShadowType = typename shadow_type<FPType>::type;
  using ShadowPtr = ShadowType **;
  static constexpr size_t VectorSize = shadow_type<FPType>::VectorSize;

  template <class ShadowPred>
  ShadowFCmp(ShadowPred Pred, ShadowPtr LeftOp, ShadowPtr RightOp)
      : Value(true), LeftOp(LeftOp), RightOp(RightOp) {
    for (int I = 0; I < VectorSize; I++) {
      Value = Value && Pred.Evaluate(LeftOp[I], RightOp[I]);
    }
  }

  operator bool() const { return Value; }
  const ShadowPtr getLeftOperand() const { return LeftOp; }
  const ShadowPtr getRightOperand() const { return RightOp; }

private:
  bool Value;
  ShadowPtr LeftOp;
  ShadowPtr RightOp;
};

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

  static bool CheckFCmp(const ShadowFCmp<FPType> &P, bool c, FPType a,
                        FPType b) {
    // We expect both comparison to be equal, else we print an error
    if (not RuntimeFlags::DisableWarning && c != P)
      FCmpCheckFail(a, P.getLeftOperand(), b, P.getRightOperand());
    // We return the shadow comparison result to be able to correctly branch
    return P;
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
