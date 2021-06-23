#include "runtime.hpp"

// ===================
// Shadow constructors
// ===================

extern "C" void __interflop_float_make_shadow(float a, shadow128_t *res) {
  interflop_make_shadow(a, res);
}

extern "C" void __interflop_float_make_v2_shadow(v2float a,
                                                 v2shadow128_t *res) {
  interflop_make_vec_shadow<v2float, v2shadow128_t, 2>(a, res);
}

extern "C" void __interflop_double_make_shadow(double a, shadow256_t *res) {
  interflop_make_shadow(a, res);
}

extern "C" void __interflop_double_make_v2_shadow(v2double a,
                                                  v2shadow256_t *res) {
  interflop_make_vec_shadow<v2double, v2shadow256_t, 2>(a, res);
}

// ===================
// Cast overload
// ===================

extern "C" void __interflop_float_double_cast(float a, shadow128_t *const sa,
                                              shadow256_t *res) {
  interflop_cast(a, sa, res);
}

extern "C" void __interflop_double_float_cast(double a, shadow256_t *const sa,
                                              shadow128_t *res) {
  interflop_cast(a, sa, res);
}

// ===================
// Unary operator
// ===================

extern "C" float __interflop_float_neg(float a, shadow128_t *const sa,
                                       shadow128_t *res) {
  return interflop_neg(a, sa, res);
}

extern "C" double __interflop_double_neg(double a, shadow256_t *const sa,
                                         shadow256_t *res) {
  return interflop_neg(a, sa, res);
}

// ================================
// float binary  operation overload
// ================================

extern "C" float __interflop_float_add(float a, shadow128_t *sa, float b,
                                       shadow128_t *sb, shadow128_t *res) {
  return BinaryOperator<float>::add(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_sub(float a, shadow128_t *sa, float b,
                                       shadow128_t *sb, shadow128_t *res) {
  return BinaryOperator<float>::sub(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_mul(float a, shadow128_t *sa, float b,
                                       shadow128_t *sb, shadow128_t *res) {
  return BinaryOperator<float>::mul(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_div(float a, shadow128_t *sa, float b,
                                       shadow128_t *sb, shadow128_t *res) {
  return BinaryOperator<float>::div(a, &sa, b, &sb, &res);
}

// =====================================
// floatbinary vector operation overload
// =====================================

extern "C" v2float __interflop_float_v2_add(v2float a, v2shadow128_t *sa,
                                            v2float b, v2shadow128_t *sb,
                                            v2shadow128_t *res) {
  return BinaryOperator<v2float>::add(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2float __interflop_float_v2_sub(v2float a, v2shadow128_t *sa,
                                            v2float b, v2shadow128_t *sb,
                                            v2shadow128_t *res) {
  return BinaryOperator<v2float>::sub(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2float __interflop_float_v2_mul(v2float a, v2shadow128_t *sa,
                                            v2float b, v2shadow128_t *sb,
                                            v2shadow128_t *res) {
  return BinaryOperator<v2float>::mul(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2float __interflop_float_v2_div(v2float a, v2shadow128_t *sa,
                                            v2float b, v2shadow128_t *sb,
                                            v2shadow128_t *res) {
  return BinaryOperator<v2float>::div(a, sa->elems, b, sb->elems, res->elems);
}

// ================================
// double binary operation overload
// ================================

extern "C" double __interflop_double_add(double a, shadow256_t *sa, double b,
                                         shadow256_t *sb, shadow256_t *res) {
  return BinaryOperator<double>::add(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_sub(double a, shadow256_t *sa, double b,
                                         shadow256_t *sb, shadow256_t *res) {
  return BinaryOperator<double>::sub(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_mul(double a, shadow256_t *sa, double b,
                                         shadow256_t *sb, shadow256_t *res) {
  return BinaryOperator<double>::mul(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_div(double a, shadow256_t *sa, double b,
                                         shadow256_t *sb, shadow256_t *res) {
  return BinaryOperator<double>::div(a, &sa, b, &sb, &res);
}

// =======================================
// double binary vector operation overload
// =======================================

extern "C" v2double __interflop_double_v2_add(v2double a, v2shadow256_t *sa,
                                              v2double b, v2shadow256_t *sb,
                                              v2shadow256_t *res) {
  return BinaryOperator<v2double>::add(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2double __interflop_double_v2_sub(v2double a, v2shadow256_t *sa,
                                              v2double b, v2shadow256_t *sb,
                                              v2shadow256_t *res) {
  return BinaryOperator<v2double>::sub(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2double __interflop_double_v2_mul(v2double a, v2shadow256_t *sa,
                                              v2double b, v2shadow256_t *sb,
                                              v2shadow256_t *res) {
  return BinaryOperator<v2double>::mul(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" v2double __interflop_double_v2_div(v2double a, v2shadow256_t *sa,
                                              v2double b, v2shadow256_t *sb,
                                              v2shadow256_t *res) {
  return BinaryOperator<v2double>::div(a, sa->elems, b, sb->elems, res->elems);
}

extern "C" int __interflop_float_check(float a, shadow128_t *sa) {
  return interflop_check(a, sa);
}

extern "C" int __interflop_double_check(double a, shadow256_t *sa) {
  return interflop_check(a, sa);
}

// ===================
// Comparisons operator
// ===================

extern "C" void __interflop_float_fcmp_eq(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  BinaryOperator<float>::fcmp_eq(a, &sa, b, &sb);
}

extern "C" void __interflop_float_fcmp_neq(float a, shadow128_t *sa, float b,
                                           shadow128_t *sb) {
  BinaryOperator<float>::fcmp_neq(a, &sa, b, &sb);
}

extern "C" void __interflop_float_fcmp_gt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  BinaryOperator<float>::fcmp_gt(a, &sa, b, &sb);
}

extern "C" void __interflop_float_fcmp_lt(float a, shadow128_t *sa, float b,
                                          shadow128_t *sb) {
  BinaryOperator<float>::fcmp_lt(a, &sa, b, &sb);
}

extern "C" void __interflop_double_fcmp_eq(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  BinaryOperator<double>::fcmp_eq(a, &sa, b, &sb);
}

extern "C" void __interflop_double_fcmp_gt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  BinaryOperator<double>::fcmp_gt(a, &sa, b, &sb);
}

extern "C" void __interflop_double_fcmp_lt(double a, shadow256_t *sa, double b,
                                           shadow256_t *sb) {
  BinaryOperator<double>::fcmp_lt(a, &sa, b, &sb);
}
