#include "Shadow.hpp"
#include "runtimes/Runtime.hpp"

// ===================
// Shadow constructors
// ===================

extern "C" void __interflop_float_make_shadow(float a, shadow128_t* res) {
  Runtime<float>::MakeShadow(a, &res);
}

extern "C" void __interflop_float_v2_make_shadow(v2float a,
                                                 v2shadow128_t res) {
  Runtime<v2float>::MakeShadow(a, res);
}

extern "C" void __interflop_float_v4_make_shadow(v2float a,
                                                 v2shadow128_t res) {
  Runtime<v2float>::MakeShadow(a, res);
}

extern "C" void __interflop_double_make_shadow(double a, shadow256_t* res) {
  Runtime<double>::MakeShadow(a, &res);
}

extern "C" void __interflop_double_v2_make_shadow(v2double a,
                                                  v2shadow256_t res) {
  Runtime<v2double>::MakeShadow(a, res);
}

extern "C" void __interflop_double_v4_make_shadow(v4double a,
                                                  v4shadow256_t res) {
  Runtime<v4double>::MakeShadow(a, res);
}

extern "C" void __interflop_double_v8_make_shadow(v8double a,
                                                  v8shadow256_t res) {
  Runtime<v8double>::MakeShadow(a, res);
}

// ===================
// Cast overload
// ===================

extern "C" void __interflop_float_double_cast(float a, shadow128_t *sa,
                                              shadow256_t *res) {
  Runtime<float>::Cast(a, &sa, &res);
}


extern "C" void __interflop_float_v2_double_cast(v2float a, v2shadow128_t sa,
                                              v2shadow256_t res) {
  Runtime<v2float>::Cast(a, sa, res);
}
extern "C" void __interflop_double_float_cast(double a, shadow256_t *sa,
                                              shadow128_t *res) {
  Runtime<double>::Cast(a, &sa, &res);
}

extern "C" void __interflop_double_v2_float_cast(v2double a, v2shadow256_t sa,
                                              v2shadow128_t res) {
  Runtime<v2double>::Cast(a, sa, res);
}

// ===================
// Unary operator
// ===================

extern "C" float __interflop_float_neg(float a, shadow128_t *sa,
                                       shadow128_t *res) {
  return Runtime<float>::Neg(a, &sa, &res);
}

extern "C" double __interflop_double_neg(double a, shadow256_t *sa,
                                         shadow256_t *res) {
  return Runtime<double>::Neg(a, &sa, &res);
}

extern "C" v2double __interflop_double_v2_neg(v2double a, v2shadow256_t sa,
                                         v2shadow256_t res) {
  return Runtime<v2double>::Neg(a, sa, res);
}


// ================================
// float binary  operation overload
// ================================

extern "C" float __interflop_float_fadd(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return Runtime<float>::Add(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fsub(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return Runtime<float>::Sub(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fmul(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return Runtime<float>::Mul(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fdiv(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return Runtime<float>::Div(a, &sa, b, &sb, &res);
}

// =====================================
// floatbinary vector operation overload
// =====================================

extern "C" v2float __interflop_float_v2_fadd(v2float a, v2shadow128_t sa,
                                             v2float b, v2shadow128_t sb,
                                             v2shadow128_t res) {
  return Runtime<v2float>::Add(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fsub(v2float a, v2shadow128_t sa,
                                             v2float b, v2shadow128_t sb,
                                             v2shadow128_t res) {
  return Runtime<v2float>::Sub(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fmul(v2float a, v2shadow128_t sa,
                                             v2float b, v2shadow128_t sb,
                                             v2shadow128_t res) {
  return Runtime<v2float>::Mul(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fdiv(v2float a, v2shadow128_t sa,
                                             v2float b, v2shadow128_t sb,
                                             v2shadow128_t res) {
  return Runtime<v2float>::Div(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fadd(v4float a, v4shadow128_t sa,
                                             v4float b, v4shadow128_t sb,
                                             v4shadow128_t res) {
  return Runtime<v4float>::Add(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fsub(v4float a, v4shadow128_t sa,
                                             v4float b, v4shadow128_t sb,
                                             v4shadow128_t res) {
  return Runtime<v4float>::Sub(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fmul(v4float a, v4shadow128_t sa,
                                             v4float b, v4shadow128_t sb,
                                             v4shadow128_t res) {
  return Runtime<v4float>::Mul(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fdiv(v4float a, v4shadow128_t sa,
                                             v4float b, v4shadow128_t sb,
                                             v4shadow128_t res) {
  return Runtime<v4float>::Div(a, sa, b, sb, res);
}

// ================================
// double binary operation overload
// ================================

extern "C" double __interflop_double_fadd(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return Runtime<double>::Add(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fsub(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return Runtime<double>::Sub(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fmul(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return Runtime<double>::Mul(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fdiv(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return Runtime<double>::Div(a, &sa, b, &sb, &res);
}

// =======================================
// double binary vector operation overload
// =======================================

extern "C" v2double __interflop_double_v2_fadd(v2double a, v2shadow256_t sa,
                                               v2double b, v2shadow256_t sb,
                                               v2shadow256_t res) {
  return Runtime<v2double>::Add(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fsub(v2double a, v2shadow256_t sa,
                                               v2double b, v2shadow256_t sb,
                                               v2shadow256_t res) {
  return Runtime<v2double>::Sub(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fmul(v2double a, v2shadow256_t sa,
                                               v2double b, v2shadow256_t sb,
                                               v2shadow256_t res) {
  return Runtime<v2double>::Mul(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fdiv(v2double a, v2shadow256_t sa,
                                               v2double b, v2shadow256_t sb,
                                               v2shadow256_t res) {
  return Runtime<v2double>::Div(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fadd(v4double a, v4shadow256_t sa,
                                               v4double b, v4shadow256_t sb,
                                               v4shadow256_t res) {
  return Runtime<v4double>::Add(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fsub(v4double a, v4shadow256_t sa,
                                               v4double b, v4shadow256_t sb,
                                               v4shadow256_t res) {
  return Runtime<v4double>::Sub(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fmul(v4double a, v4shadow256_t sa,
                                               v4double b, v4shadow256_t sb,
                                               v4shadow256_t res) {
  return Runtime<v4double>::Mul(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fdiv(v4double a, v4shadow256_t sa,
                                               v4double b, v4shadow256_t sb,
                                               v4shadow256_t res) {
  return Runtime<v4double>::Div(a, sa, b, sb, res);
}

extern "C" int __interflop_float_check(float a, shadow128_t *sa) {
  return Runtime<float>::Check(a, &sa);
}

extern "C" int __interflop_double_check(double a, shadow256_t *sa) {
  return Runtime<double>::Check(a, &sa);
}

// =============================
// Oredered Comparisons operator
// =============================

extern "C" int __interflop_float_fcmp_oeq(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_oeq>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_float_fcmp_one(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_one>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_float_fcmp_ogt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_ogt>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_float_fcmp_olt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_olt>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_oeq(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_oeq>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_one(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_one>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_ogt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_ogt>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_olt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_olt>(), &sa, &sb), a == b, a, b);
}

// =============================
// Unoredered Comparisons operator
// =============================

extern "C" int __interflop_float_fcmp_ueq(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_ueq>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_float_fcmp_une(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_une>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_float_fcmp_ugt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_ugt>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_float_fcmp_ult(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return Runtime<float>::CheckFCmp(
      ShadowFCmp<float>(ShadowPredicate<float, kFCmp_ult>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_ueq(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_ueq>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_une(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_une>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_ugt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_ugt>(), &sa, &sb), a == b, a, b);
}

extern "C" int __interflop_double_fcmp_ult(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return Runtime<double>::CheckFCmp(
      ShadowFCmp<double>(ShadowPredicate<double, kFCmp_ult>(), &sa, &sb), a == b, a, b);
}