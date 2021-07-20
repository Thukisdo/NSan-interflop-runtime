/**
 * @file Interface.cpp
 * @author Mathys JAM (mathys.jam@gmail.com)
 * @brief C interface for the library.
 * @version 0.7.0
 * @date 2021-07-20
 * 
 */

#define BACKEND_DEFINITION
#include "Context.hpp"

// This file is filled with boilerplate code that declares the interface between
// the instrumented code and the backends
// The following should not be changed unless either side of the interface
// changes.
//
// Backends should rely on the provided backend interface and should
// not modify this file.

// The following functions should be defined outside interflop::
using namespace interflop;

// ===================
// Shadow constructors
// ===================

extern "C" void __interflop_float_make_shadow(float a, OpaqueShadow128 *res) {
  Context.getBackendFor<float>().MakeShadow(a, &res);
}

extern "C" void __interflop_float_v2_make_shadow(v2float a,
                                                 v2OpaqueShadow128 res) {
  Context.getBackendFor<v2float>().MakeShadow(a, res);
}

extern "C" void __interflop_float_v4_make_shadow(v4float a,
                                                 v4OpaqueShadow128 res) {
  Context.getBackendFor<v4float>().MakeShadow(a, res);
}

extern "C" void __interflop_double_make_shadow(double a, OpaqueShadow256 *res) {
  Context.getBackendFor<double>().MakeShadow(a, &res);
}

extern "C" void __interflop_double_v2_make_shadow(v2double a,
                                                  v2OpaqueShadow256 res) {
  Context.getBackendFor<v2double>().MakeShadow(a, res);
}

extern "C" void __interflop_double_v4_make_shadow(v4double a,
                                                  v4OpaqueShadow256 res) {
  Context.getBackendFor<v4double>().MakeShadow(a, res);
}

extern "C" void __interflop_double_v8_make_shadow(v8double a,
                                                  v8OpaqueShadow256 res) {
  Context.getBackendFor<v8double>().MakeShadow(a, res);
}

// ===================
// Cast overload
// ===================

extern "C" void __interflop_float_double_cast(float a, OpaqueShadow128 *sa,
                                              OpaqueShadow256 *res) {
  Context.getBackendFor<float>().UpCast(a, &sa, &res);
}

extern "C" void __interflop_float_v2_double_cast(v2float a,
                                                 v2OpaqueShadow128 sa,
                                                 v2OpaqueShadow256 res) {
  Context.getBackendFor<v2float>().UpCast(a, sa, res);
}
extern "C" void __interflop_double_float_cast(double a, OpaqueShadow256 *sa,
                                              OpaqueShadow128 *res) {
  Context.getBackendFor<double>().DownCast(a, &sa, &res);
}

extern "C" void __interflop_double_v2_float_cast(v2double a,
                                                 v2OpaqueShadow256 sa,
                                                 v2OpaqueShadow128 res) {
  Context.getBackendFor<v2double>().DownCast(a, sa, res);
}

// ===================
// Unary operator
// ===================

extern "C" float __interflop_float_neg(float a, OpaqueShadow128 *sa,
                                       OpaqueShadow128 *res) {
  return Context.getBackendFor<float>().Neg(a, &sa, &res);
}

extern "C" double __interflop_double_neg(double a, OpaqueShadow256 *sa,
                                         OpaqueShadow256 *res) {
  return Context.getBackendFor<double>().Neg(a, &sa, &res);
}

extern "C" v2double __interflop_double_v2_neg(v2double a, v2OpaqueShadow256 sa,
                                              v2OpaqueShadow256 res) {
  return Context.getBackendFor<v2double>().Neg(a, sa, res);
}

// ================================
// float binary  operation overload
// ================================

extern "C" float __interflop_float_fadd(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  return Context.getBackendFor<float>().Add(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fsub(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  return Context.getBackendFor<float>().Sub(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fmul(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  return Context.getBackendFor<float>().Mul(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fdiv(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  return Context.getBackendFor<float>().Div(a, &sa, b, &sb, &res);
}

// =====================================
// floatbinary vector operation overload
// =====================================

extern "C" v2float __interflop_float_v2_fadd(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  return Context.getBackendFor<v2float>().Add(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fsub(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  return Context.getBackendFor<v2float>().Sub(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fmul(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  return Context.getBackendFor<v2float>().Mul(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fdiv(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  return Context.getBackendFor<v2float>().Div(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fadd(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  return Context.getBackendFor<v4float>().Add(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fsub(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  return Context.getBackendFor<v4float>().Sub(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fmul(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  return Context.getBackendFor<v4float>().Mul(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fdiv(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  return Context.getBackendFor<v4float>().Div(a, sa, b, sb, res);
}

// ================================
// double binary operation overload
// ================================

extern "C" double __interflop_double_fadd(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  return Context.getBackendFor<double>().Add(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fsub(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  return Context.getBackendFor<double>().Sub(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fmul(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  return Context.getBackendFor<double>().Mul(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fdiv(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  return Context.getBackendFor<double>().Div(a, &sa, b, &sb, &res);
}

// =======================================
// double binary vector operation overload
// =======================================

extern "C" v2double __interflop_double_v2_fadd(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  return Context.getBackendFor<v2double>().Add(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fsub(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  return Context.getBackendFor<v2double>().Sub(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fmul(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  return Context.getBackendFor<v2double>().Mul(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fdiv(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  return Context.getBackendFor<v2double>().Div(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fadd(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  return Context.getBackendFor<v4double>().Add(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fsub(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  return Context.getBackendFor<v4double>().Sub(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fmul(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  return Context.getBackendFor<v4double>().Mul(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fdiv(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  return Context.getBackendFor<v4double>().Div(a, sa, b, sb, res);
}

extern "C" int __interflop_float_check(float a, OpaqueShadow128 *sa) {
  return Context.getBackendFor<float>().Check(a, &sa);
}

extern "C" int __interflop_double_check(double a, OpaqueShadow256 *sa) {
  return Context.getBackendFor<double>().Check(a, &sa);
}

// =============================
// Oredered Comparisons operator
// =============================

namespace {
// Comparing two vector yields a vector of bool
// We need to reduce it to a singleton before continuing
template <typename ScalarT, typename VectorT>
bool ReducePredicate(VectorT Vec) {
  // The vector of bool is the same size as the base vector
  // So we can get the number of element by dividing both size
  constexpr size_t Size = sizeof(VectorT) / sizeof(ScalarT);
  bool Res = true;

  // We reduce the vector to a singleton
  for (int I = 0; Res && (I < Size); I++)
    Res = Res && Vec[I];
  return Res;
}

// We may be able to replace this boilerplate code with function aliasing using
// some templates ?

} // namespace

// Float Ordered Comparisons

extern "C" int __interflop_float_fcmp_oeq(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_oeq, a, &sa, b, &sb,
                                                  a == b);
}

extern "C" int __interflop_float_fcmp_one(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_one, a, &sa, b, &sb,
                                                  a != b);
}

extern "C" int __interflop_float_fcmp_ogt(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_ogt, a, &sa, b, &sb,
                                                  a > b);
}

extern "C" int __interflop_float_fcmp_olt(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_olt, a, &sa, b, &sb,
                                                  a < b);
}

//
//
// Float Vector Comparisons
//
//

extern "C" int __interflop_float_v2_fcmp_oeq(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  return Context.getBackendFor<v2float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2float>(a == b));
}

extern "C" int __interflop_float_v2_fcmp_one(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  return Context.getBackendFor<v2float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2float>(a != b));
}

extern "C" int __interflop_float_v2_fcmp_ogt(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  return Context.getBackendFor<v2float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2float>(a > b));
}

extern "C" int __interflop_float_v2_fcmp_olt(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  return Context.getBackendFor<v2float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2float>(a < b));
}

extern "C" int __interflop_float_v4_fcmp_oeq(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  return Context.getBackendFor<v4float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4float>(a == b));
}

extern "C" int __interflop_float_v4_fcmp_one(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  return Context.getBackendFor<v4float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4float>(a != b));
}

extern "C" int __interflop_float_v4_fcmp_ogt(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  return Context.getBackendFor<v4float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4float>(a > b));
}
extern "C" int __interflop_float_v4_fcmp_olt(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  return Context.getBackendFor<v4float>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4float>(a < b));
}

//
//
// Double Vector Comparisons
//
//

extern "C" int __interflop_double_fcmp_oeq(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_oeq, a, &sa, b, &sb,
                                                   a == b);
}

extern "C" int __interflop_double_fcmp_one(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_one, a, &sa, b, &sb,
                                                   a != b);
}

extern "C" int __interflop_double_fcmp_ogt(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_ogt, a, &sa, b, &sb,
                                                   a > b);
}

extern "C" int __interflop_double_fcmp_olt(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_olt, a, &sa, b, &sb,
                                                   a < b);
}

//
//
// Double Vector Comparisons
//
//

extern "C" int __interflop_double_v2_fcmp_oeq(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  return Context.getBackendFor<v2double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2double>(a == b));
}

extern "C" int __interflop_double_v2_fcmp_one(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  return Context.getBackendFor<v2double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2double>(a != b));
}

extern "C" int __interflop_double_v2_fcmp_ogt(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  return Context.getBackendFor<v2double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2double>(a > b));
}

extern "C" int __interflop_double_v2_fcmp_olt(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  return Context.getBackendFor<v2double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v2double>(a < b));
}

extern "C" int __interflop_double_v4_fcmp_oeq(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  return Context.getBackendFor<v4double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4double>(a == b));
}

extern "C" int __interflop_double_v4_fcmp_one(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  return Context.getBackendFor<v4double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4double>(a != b));
}

extern "C" int __interflop_double_v4_fcmp_ogt(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  return Context.getBackendFor<v4double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4double>(a > b));
}
extern "C" int __interflop_double_v4_fcmp_olt(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  return Context.getBackendFor<v4double>().CheckFCmp(
      FCmp_oeq, a, sa, b, sb, ReducePredicate<v4double>(a < b));
}

// =============================
// Unoredered Comparisons operator
// =============================

extern "C" int __interflop_float_fcmp_ueq(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_ueq, a, &sa, b, &sb,
                                                  a == b);
}

extern "C" int __interflop_float_fcmp_une(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_une, a, &sa, b, &sb,
                                                  a != b);
}

extern "C" int __interflop_float_fcmp_ugt(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_ugt, a, &sa, b, &sb,
                                                  a > b);
}

extern "C" int __interflop_float_fcmp_ult(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  return Context.getBackendFor<float>().CheckFCmp(FCmp_ult, a, &sa, b, &sb,
                                                  a < b);
}

extern "C" int __interflop_double_fcmp_ueq(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_ueq, a, &sa, b, &sb,
                                                   a == b);
}

extern "C" int __interflop_double_fcmp_une(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_une, a, &sa, b, &sb,
                                                   a != b);
}

extern "C" int __interflop_double_fcmp_ugt(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_ugt, a, &sa, b, &sb,
                                                   a > b);
}

extern "C" int __interflop_double_fcmp_ult(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  return Context.getBackendFor<double>().CheckFCmp(FCmp_ult, a, &sa, b, &sb,
                                                   a < b);
}