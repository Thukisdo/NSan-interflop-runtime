#include "runtime.hpp"

// ===================
// Shadow constructors
// ===================

extern "C" void __interflop_float_make_shadow(float a, shadow128_t *res) {
  RuntimeUtils<float>::make_shadow(a, &res);
}

extern "C" void __interflop_float_v2_make_shadow(v2float a,
                                                 v2shadow128_t *res) {
  RuntimeUtils<v2float>::make_shadow(a, res->elems);
}

extern "C" void __interflop_float_v4_make_shadow(v2float a,
                                                 v2shadow128_t *res) {
  RuntimeUtils<v2float>::make_shadow(a, res->elems);
}

extern "C" void __interflop_double_make_shadow(double a, shadow256_t *res) {
  RuntimeUtils<double>::make_shadow(a, &res);
}

extern "C" void __interflop_double_v2_make_shadow(v2double a,
                                                  v2shadow256_t *res) {
  RuntimeUtils<v2double>::make_shadow(a, res->elems);
}

extern "C" void __interflop_double_v4_make_shadow(v4double a,
                                                  v4shadow256_t *res) {
  RuntimeUtils<v4double>::make_shadow(a, res->elems);
}

extern "C" void __interflop_double_v8_make_shadow(v8double a,
                                                  v8shadow256_t *res) {
  RuntimeUtils<v8double>::make_shadow(a, res->elems);
}

// ===================
// Cast overload
// ===================

extern "C" void __interflop_float_double_cast(float a, shadow128_t *sa,
                                              shadow256_t *res) {
  RuntimeOperator<float>::cast(a, &sa, &res);
}


extern "C" void __interflop_float_v2_double_cast(v2float a, v2shadow128_t *sa,
                                              v2shadow256_t *res) {
  RuntimeOperator<v2float>::cast(a, sa->elems, res->elems);
}
extern "C" void __interflop_double_float_cast(double a, shadow256_t *sa,
                                              shadow128_t *res) {
  RuntimeOperator<double>::cast(a, &sa, &res);
}

extern "C" void __interflop_double_v2_float_cast(v2double a, v2shadow256_t *sa,
                                              v2shadow128_t *res) {
  RuntimeOperator<v2double>::cast(a, sa->elems, res->elems);
}

// ===================
// Unary operator
// ===================

extern "C" float __interflop_float_neg(float a, shadow128_t *sa,
                                       shadow128_t *res) {
  return RuntimeOperator<float>::neg(a, &sa, &res);
}

extern "C" double __interflop_double_neg(double a, shadow256_t *sa,
                                         shadow256_t *res) {
  return RuntimeOperator<double>::neg(a, &sa, &res);
}

extern "C" v2double __interflop_double_v2_neg(v2double a, v2shadow256_t *sa,
                                         v2shadow256_t *res) {
  return RuntimeOperator<v2double>::neg(a, sa->elems, res->elems);
}


// ================================
// float binary  operation overload
// ================================

extern "C" float __interflop_float_fadd(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return RuntimeOperator<float>::add(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fsub(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return RuntimeOperator<float>::sub(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fmul(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return RuntimeOperator<float>::mul(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fdiv(float a, shadow128_t *sa, float b,
                                        shadow128_t *sb, shadow128_t *res) {
  return RuntimeOperator<float>::div(a, &sa, b, &sb, &res);
}

// =====================================
// floatbinary vector operation overload
// =====================================

extern "C" v2float __interflop_float_v2_fadd(v2float a, v2shadow128_t *sa,
                                             v2float b, v2shadow128_t *sb,
                                             v2shadow128_t *res) {
  return RuntimeOperator<v2float>::add(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2float __interflop_float_v2_fsub(v2float a, v2shadow128_t *sa,
                                             v2float b, v2shadow128_t *sb,
                                             v2shadow128_t *res) {
  return RuntimeOperator<v2float>::sub(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2float __interflop_float_v2_fmul(v2float a, v2shadow128_t *sa,
                                             v2float b, v2shadow128_t *sb,
                                             v2shadow128_t *res) {
  return RuntimeOperator<v2float>::mul(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2float __interflop_float_v2_fdiv(v2float a, v2shadow128_t *sa,
                                             v2float b, v2shadow128_t *sb,
                                             v2shadow128_t *res) {
  return RuntimeOperator<v2float>::div(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4float __interflop_float_v4_fadd(v4float a, v4shadow128_t *sa,
                                             v4float b, v4shadow128_t *sb,
                                             v4shadow128_t *res) {
  return RuntimeOperator<v4float>::add(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4float __interflop_float_v4_fsub(v4float a, v4shadow128_t *sa,
                                             v4float b, v4shadow128_t *sb,
                                             v4shadow128_t *res) {
  return RuntimeOperator<v4float>::sub(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4float __interflop_float_v4_fmul(v4float a, v4shadow128_t *sa,
                                             v4float b, v4shadow128_t *sb,
                                             v4shadow128_t *res) {
  return RuntimeOperator<v4float>::mul(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4float __interflop_float_v4_fdiv(v4float a, v4shadow128_t *sa,
                                             v4float b, v4shadow128_t *sb,
                                             v4shadow128_t *res) {
  return RuntimeOperator<v4float>::div(a, sa->elems, b, sb->elems, res->elems);
}

// ================================
// double binary operation overload
// ================================

extern "C" double __interflop_double_fadd(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return RuntimeOperator<double>::add(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fsub(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return RuntimeOperator<double>::sub(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fmul(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return RuntimeOperator<double>::mul(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fdiv(double a, shadow256_t *sa, double b,
                                          shadow256_t *sb, shadow256_t *res) {
  return RuntimeOperator<double>::div(a, &sa, b, &sb, &res);
}

// =======================================
// double binary vector operation overload
// =======================================

extern "C" v2double __interflop_double_v2_fadd(v2double a, v2shadow256_t *sa,
                                               v2double b, v2shadow256_t *sb,
                                               v2shadow256_t *res) {
  return RuntimeOperator<v2double>::add(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2double __interflop_double_v2_fsub(v2double a, v2shadow256_t *sa,
                                               v2double b, v2shadow256_t *sb,
                                               v2shadow256_t *res) {
  return RuntimeOperator<v2double>::sub(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2double __interflop_double_v2_fmul(v2double a, v2shadow256_t *sa,
                                               v2double b, v2shadow256_t *sb,
                                               v2shadow256_t *res) {
  return RuntimeOperator<v2double>::mul(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2double __interflop_double_v2_fdiv(v2double a, v2shadow256_t *sa,
                                               v2double b, v2shadow256_t *sb,
                                               v2shadow256_t *res) {
  return RuntimeOperator<v2double>::div(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4double __interflop_double_v4_fadd(v4double a, v4shadow256_t *sa,
                                               v4double b, v4shadow256_t *sb,
                                               v4shadow256_t *res) {
  return RuntimeOperator<v4double>::add(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4double __interflop_double_v4_fsub(v4double a, v4shadow256_t *sa,
                                               v4double b, v4shadow256_t *sb,
                                               v4shadow256_t *res) {
  return RuntimeOperator<v4double>::sub(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4double __interflop_double_v4_fmul(v4double a, v4shadow256_t *sa,
                                               v4double b, v4shadow256_t *sb,
                                               v4shadow256_t *res) {
  return RuntimeOperator<v4double>::mul(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v4double __interflop_double_v4_fdiv(v4double a, v4shadow256_t *sa,
                                               v4double b, v4shadow256_t *sb,
                                               v4shadow256_t *res) {
  return RuntimeOperator<v4double>::div(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" int __interflop_float_check(float a, shadow128_t *sa) {
  return RuntimeUtils<float>::check(a, &sa);
}

extern "C" int __interflop_double_check(double a, shadow256_t *sa) {
  return RuntimeUtils<double>::check(a, &sa);
}

// =============================
// Oredered Comparisons operator
// =============================

extern "C" int __interflop_float_fcmp_oeq(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      OrderedFPPredicate<fcmp_eq, float>(&sa, &sb), a == b, a, &sa, b, &sb);
}

extern "C" int __interflop_float_fcmp_one(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      OrderedFPPredicate<fcmp_ne, float>(&sa, &sb), a != b, a, &sa, b, &sb);
}

extern "C" int __interflop_float_fcmp_ogt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      OrderedFPPredicate<fcmp_gt, float>(&sa, &sb), a > b, a, &sa, b, &sb);
}

extern "C" int __interflop_float_fcmp_olt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      OrderedFPPredicate<fcmp_lt, float>(&sa, &sb), a < b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_oeq(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      OrderedFPPredicate<fcmp_eq, double>(&sa, &sb), a == b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_one(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      OrderedFPPredicate<fcmp_ne, double>(&sa, &sb), a != b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_ogt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      OrderedFPPredicate<fcmp_gt, double>(&sa, &sb), a > b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_olt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      OrderedFPPredicate<fcmp_lt, double>(&sa, &sb), a < b, a, &sa, b, &sb);
}

// =============================
// Unoredered Comparisons operator
// =============================

extern "C" int __interflop_float_fcmp_ueq(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      UnorderedFPPredicate<fcmp_eq, float>(&sa, &sb), a == b, a, &sa, b, &sb);
}

extern "C" int __interflop_float_fcmp_une(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      UnorderedFPPredicate<fcmp_ne, float>(&sa, &sb), a != b, a, &sa, b, &sb);
}

extern "C" int __interflop_float_fcmp_ugt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      UnorderedFPPredicate<fcmp_gt, float>(&sa, &sb),a > b,  a, &sa, b, &sb);
}

extern "C" int __interflop_float_fcmp_ult(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  return RuntimeOperator<float>::fcmp(
      UnorderedFPPredicate<fcmp_lt, float>(&sa, &sb), a < b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_ueq(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      UnorderedFPPredicate<fcmp_eq, double>(&sa, &sb), a == b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_une(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      UnorderedFPPredicate<fcmp_ne, double>(&sa, &sb), a != b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_ugt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      UnorderedFPPredicate<fcmp_gt, double>(&sa, &sb), a > b, a, &sa, b, &sb);
}

extern "C" int __interflop_double_fcmp_ult(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  return RuntimeOperator<double>::fcmp(
      UnorderedFPPredicate<fcmp_lt, double>(&sa, &sb), a < b, a, &sa, b, &sb);
}