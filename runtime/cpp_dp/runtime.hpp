#pragma once
#include "shadow.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

constexpr bool exit_on_error = false;
constexpr bool output_warning = false;

void __nsan_dump_stacktrace();

namespace {
void dump_stacktrace() {
  printf("\n\nInterflop stacktrace : \n");
  __nsan_dump_stacktrace();
  printf("\n\n");
}
} // namespace

enum FCMP_Pred { fcmp_eq, fcmp_ne, fcmp_gt, fcmp_lt };

class Predicate {
public:
  operator bool() const { return value; }

  bool getValue() const { return value; }

protected:
  bool value = true;
};

template <FCMP_Pred Pred, bool Ordered, typename VT>
class FPPredicate : public Predicate {
public:
  using SVT = typename shadow_type<VT>::type;
  static constexpr size_t kSize = shadow_type<VT>::vector_size;
  using ShadowPtr = SVT **;

  FPPredicate(ShadowPtr sa, ShadowPtr sb) {
    for (int i = 0; i < kSize; i++) {
      bool curr = true;

      if constexpr (Pred == fcmp_eq) {
        for (int i = 0; i < kSize; i++)
          curr = (sa[i]->val == sb[i]->val);
      } else if constexpr (Pred == fcmp_ne) {
        for (int i = 0; i < kSize; i++)
          curr = (sa[i]->val != sb[i]->val);
      } else if constexpr (Pred == fcmp_lt) {
        for (int i = 0; i < kSize; i++)
          curr = (sa[i]->val < sb[i]->val);
      } else if constexpr (Pred == fcmp_gt) {
        for (int i = 0; i < kSize; i++)
          curr = (sa[i]->val > sb[i]->val);
      }
      value = value && curr;
    }
  }
};

template <FCMP_Pred Pred, typename VT>
using OrderedFPPredicate = FPPredicate<Pred, true, VT>;

template <FCMP_Pred Pred, typename VT>
using UnorderedFPPredicate = FPPredicate<Pred, false, VT>;

template <typename VT> class RuntimeOperator {
public:
  using SVT = typename shadow_type<VT>::type;
  static constexpr size_t kSize = shadow_type<VT>::vector_size;
  using ShadowPtr = SVT **;

  RuntimeOperator() = delete;
  RuntimeOperator(RuntimeOperator const &) = delete;

  static VT add(VT a, ShadowPtr const sa, VT b, ShadowPtr const sb,
                ShadowPtr res) {
    for (int i = 0; i < kSize; ++i) {
      res[i]->val = sa[i]->val + sb[i]->val;
    }
    return a + b;
  }

  static VT sub(VT a, ShadowPtr sa, VT b, ShadowPtr sb, ShadowPtr res) {
    for (int i = 0; i < kSize; ++i) {
      res[i]->val = sa[i]->val - sb[i]->val;
    }
    return a - b;
  }

  static VT mul(VT a, ShadowPtr sa, VT b, ShadowPtr sb, ShadowPtr res) {
    for (int i = 0; i < kSize; ++i) {
      res[i]->val = sa[i]->val * sb[i]->val;
    }
    return a * b;
  }

  static VT div(VT a, ShadowPtr sa, VT b, ShadowPtr sb, ShadowPtr res) {

    for (int i = 0; i < kSize; ++i) {
      res[i]->val = sa[i]->val / sb[i]->val;
    }
    return a / b;
  }

  static void fcmp_err(VT a, ShadowPtr sa, VT b, ShadowPtr sb) {
    printf("floating-point comparison results depend on precision\n");
    printf("\tValue a: %.20lf b: %.20lf\n", a, b);
    for (int i = 0; i < kSize; i++) {
      printf("\tSHADOW Value(s) a%i: %.20lf b%i: %.20lf\n", i,
             (double)sa[i]->val, i, (double)sb[i]->val);
    }
    dump_stacktrace();
    if constexpr (exit_on_error)
      exit(1);
  }

  static int fcmp(Predicate const &P, bool c, VT a, ShadowPtr sa, VT b,
                  ShadowPtr sb) {
    if (output_warning && c != P)
      fcmp_err(a, sa, b, sb);

    return P.getValue();
  }

  template <typename DestVT>
  static void cast(VT a, ShadowPtr sa, DestVT **res) {
    for (int i = 0; i < kSize; i++) {
      res[i]->val = sa[i]->val;
    }
  }

  static VT neg(VT a, ShadowPtr sa, ShadowPtr res) {
    for (int i = 0; i < kSize; i++) {
      res[i]->val = -sa[i]->val;
    }
    return -a;
  }

private:
};

template <typename VT> class RuntimeUtils {
public:
  using SVT = typename shadow_type<VT>::type;
  using ScalarVT = typename shadow_type<VT>::scalar_type;
  static constexpr size_t kSize = shadow_type<VT>::vector_size;
  using ShadowPtr = SVT **;

  static void make_shadow(VT a, ShadowPtr res) {
    for (int i = 0; i < kSize; i++) {
      if constexpr (kSize > 1)
        res[i]->val = a[i];
      else
        res[0]->val = a;
    }
  }

  static int check(VT a, ShadowPtr sa) {

    // We unvectorize the check
    if constexpr (kSize > 1) {
      bool res = 0;
      for (int i = 0; i < kSize; i++) {
        if (not sa[i])
          continue;
        res = res || RuntimeUtils<ScalarVT>::check(a[i], &sa[i]);
      }
      return res;
    }

    double AbsoluteErrror = a - sa[0]->val;
    double RelativeError = (AbsoluteErrror / sa[0]->val) * 100;
    if (AbsoluteErrror < 0)
      AbsoluteErrror *= -1;
    if (RelativeError < 0)
      RelativeError *= -1;

    if (RelativeError >= 0.0001) {
      if (output_warning) {
        printf("\033[1;31m");
        printf("Inconsistent shadow result : expected %lf got %lf\n",
               (double)sa[0]->val, (double)a);
        printf("\tNative Value %lf\n", (double)a);
        printf("\tShadow Value %lf\n", (double)sa[0]->val);
        printf("\tRelative error %lf\n", RelativeError);
        printf("\tAbsolute error %lf\n", AbsoluteErrror);
        printf("\033[0m");
        dump_stacktrace();
      }
      if constexpr (exit_on_error)
        exit(1);
      return 1;
    }
    return 0;
  }
};
