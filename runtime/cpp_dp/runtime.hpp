#pragma once
#include "shadow.hpp"
#include <iomanip>
#include <iostream>
#include <limits>

void __nsan_dump_stacktrace();

namespace {
    void dump_stacktrace()
    {
        std::cout << std::endl << "Interflop stacktrace : " << std::endl;
        __nsan_dump_stacktrace();
        std::cout << std::endl << std::endl; 
    }
}


template <typename VT> class BinaryOperator {
public:
  using SVT = typename shadow_type<VT>::type;
  static constexpr size_t kSize = shadow_type<VT>::vector_size;
  using ShadowPtr = SVT **;

  BinaryOperator() = delete;
  BinaryOperator(BinaryOperator const &) = delete;

  static VT add(VT a, ShadowPtr const sa, VT b, ShadowPtr const sb,
                ShadowPtr res) {
    for (int i = 0; i < kSize; ++i) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res[i]->val = sa[i]->val + sb[i]->val;
    }
    return a + b;
  }

  static VT sub(VT a, ShadowPtr sa, VT b, ShadowPtr sb, ShadowPtr res) {
    for (int i = 0; i < kSize; ++i) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res[i]->val = sa[i]->val - sb[i]->val;
    }
    return a - b;
  }

  static VT mul(VT a, ShadowPtr sa, VT b, ShadowPtr sb, ShadowPtr res) {
    for (int i = 0; i < kSize; ++i) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res[i]->val = sa[i]->val * sb[i]->val;
    }
    return a * b;
  }

  static VT div(VT a, ShadowPtr sa, VT b, ShadowPtr sb, ShadowPtr res) {

    for (int i = 0; i < kSize; ++i) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res[i]->val = sa[i]->val / sb[i]->val;
    }
    return a / b;
  }

  static void fcmp_eq(VT a, ShadowPtr sa, VT b, ShadowPtr sb) {
    bool res = true;

    for (int i = 0; i < kSize; i++) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res = res && (sa[i]->val == sb[i]->val);
    }

    if ((a == b) != res) {
      std::cout << "floating-point comparison results depend on precision"
                << std::endl;
      std::cout << "\tValue a: " << a << " b: " << b << std::endl;
      for (int i = 0; i < kSize; i++) {
        // Same safety as above
        if constexpr (kSize != 1)
          if (not sa[i] || not sb[i])
            continue;
        std::cout << "\tSHADOW Value(s) a" << i << ": "
                  << static_cast<double>(sa[i]->val) << " b" << i << ": "
                  << static_cast<double>(sb[i]->val) << std::endl;
      }
      dump_stacktrace();
      exit(1);
    }
  }

  static void fcmp_neq(VT a, ShadowPtr sa, VT b, ShadowPtr sb) {
    bool res = true;

    for (int i = 0; i < kSize; i++) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res = res && (sa[i]->val != sb[i]->val);
    }

    if ((a != b) != res) {
      printf("floating-point comparison results depend on precision\n");
      printf("\tValue (%.20f)\n", a);
      for (int i = 0; i < kSize; i++) {
        // Same safety as above
        if constexpr (kSize != 1)
          if (not sa[i] || not sb[i])
            continue;
        std::cout << "\tSHADOW Value(s) a" << i << ": "
                  << static_cast<double>(sa[i]->val) << " b" << i << ": "
                  << static_cast<double>(sb[i]->val) << std::endl;
      }
      dump_stacktrace();
      exit(1);
    }
  }

  static void fcmp_gt(VT a, ShadowPtr sa, float b, ShadowPtr sb) {
    bool res = true;

    for (int i = 0; i < kSize; i++) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res = res && (sa[i]->val > sb[i]->val);
    }

    if ((a > b) != res) {
      printf("floating-point comparison results depend on precision\n");
      printf("\tValue (%.20f)\n", a);
      for (int i = 0; i < kSize; i++) {
        // Same safety as above
        if constexpr (kSize != 1)
          if (not sa[i] || not sb[i])
            continue;
        std::cout << "\tSHADOW Value(s) a" << i << ": "
                  << static_cast<double>(sa[i]->val) << " b" << i << ": "
                  << static_cast<double>(sb[i]->val) << std::endl;
      }
      dump_stacktrace();
      exit(1);
    }
  }

  static void fcmp_lt(VT a, ShadowPtr sa, VT b, ShadowPtr sb) {
    bool res = true;

    for (int i = 0; i < kSize; i++) {
      // When working with vectors, we want to prevent NULL ptr dereferencing in
      // case of a poison value
      if constexpr (kSize != 1)
        if (not sa[i] || not sb[i])
          continue;
      res = res && (sa[i]->val < sb[i]->val);
    }

    if ((a < b) != res) {
      printf("floating-point comparison results depend on precision\n");
      printf("\tValue (%.20f)\n", a);
      for (int i = 0; i < kSize; i++) {
        // Same safety as above
        if constexpr (kSize != 1)
          if (not sa[i] || not sb[i])
            continue;
        std::cout << "\tSHADOW Value(s) a" << i << ": "
                  << static_cast<double>(sa[i]->val) << " b" << i << ": "
                  << static_cast<double>(sb[i]->val) << std::endl;
      }
      dump_stacktrace();
      exit(1);
    }
  }

private:
};

template <typename VT, typename SVT, size_t n>
inline void interflop_make_vec_shadow(VT a, SVT *res) {
  for (int i = 0; i < n; ++i)
    res->elems[i]->val = a[i];
}

template <typename VT, typename SVT>
inline void interflop_make_shadow(VT a, SVT *res) {
  res->val = a;
}

template <typename VT, typename SVT> int interflop_check(VT a, SVT *sa) {
  double AbsoluteErrror = a - sa->val;
  double RelativeError = (AbsoluteErrror / sa->val) * 100;
  if (AbsoluteErrror < 0)
    AbsoluteErrror *= -1;
  if (RelativeError < 0)
    RelativeError *= -1;

  if (RelativeError >= 0.0001) {
    std::cout << "\033[1;31m";
    std::cout << "Inconsistent shadow result : expected "
              << std::setprecision(std::numeric_limits<double>::digits10 + 1)
              << static_cast<double>(sa->val) << ", value is " << a
              << std::endl;
    std::cout << "\tNative Value " << a << std::endl;
    std::cout << "\tShadow Value " << static_cast<double>(sa->val) << std::endl;
    std::cout << "\tRelative error " << RelativeError << std::endl;
    std::cout << "\tAbsolute error " << AbsoluteErrror << std::endl;
    std::cout << "\033[0m";
    dump_stacktrace();
    exit(1);
    return 1;
  }
  return 0;
}

template <typename VT, typename SVT, typename DSVT>
void interflop_cast(VT a, SVT const *sa, DSVT *res) {
  res->val = sa->val;
}

template <typename VT, typename SVT>
VT interflop_neg(VT a, SVT *const sa, SVT *res) {
  res->val = -sa->val;
  return -a;
}
