/**
 * @file Interface.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
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

template<typename T>
using Backend = InterflopBackend<T>;

// ===================
// Shadow constructors
// ===================

extern "C" void __interflop_float_make_shadow(float a, OpaqueShadow128 *res) {
  Backend<float> Backend;
  Backend.MakeShadow(a, &res);
}

extern "C" void __interflop_float_v2_make_shadow(v2float a,
                                                 v2OpaqueShadow128 res) {
  Backend<v2float> Backend;
  Backend.MakeShadow(a, res);
}

extern "C" void __interflop_float_v4_make_shadow(v4float a,
                                                 v4OpaqueShadow128 res) {
  Backend<v4float> Backend;
  Backend.MakeShadow(a, res);
}

extern "C" void __interflop_float_v8_make_shadow(v8float a,
                                                 v8OpaqueShadow128 res) {
  Backend<v8float> Backend;
  Backend.MakeShadow(a, res);
}

extern "C" void __interflop_double_make_shadow(double a, OpaqueShadow256 *res) {
  Backend<double> Backend;
  Backend.MakeShadow(a, &res);
}

extern "C" void __interflop_double_v2_make_shadow(v2double a,
                                                  v2OpaqueShadow256 res) {
  Backend<v2double> Backend;
  Backend.MakeShadow(a, res);
}

extern "C" void __interflop_double_v4_make_shadow(v4double a,
                                                  v4OpaqueShadow256 res) {
  Backend<v4double> Backend;
  Backend.MakeShadow(a, res);
}

extern "C" void __interflop_double_v8_make_shadow(v8double a,
                                                  v8OpaqueShadow256 res) {
  Backend<v8double> Backend;
  Backend.MakeShadow(a, res);
}

// ===================
// Cast overload
// ===================

extern "C" void __interflop_float_double_cast(float a, OpaqueShadow128 *sa,
                                              OpaqueShadow256 *res) {
  Backend<float> Backend;
  Backend.UpCast(a, &sa, &res);
}

extern "C" void __interflop_float_v2_double_cast(v2float a,
                                                 v2OpaqueShadow128 sa,
                                                 v2OpaqueShadow256 res) {
  Backend<v2float> Backend;
  Backend.UpCast(a, sa, res);
}
extern "C" void __interflop_double_float_cast(double a, OpaqueShadow256 *sa,
                                              OpaqueShadow128 *res) {
  Backend<double> Backend;
  Backend.DownCast(a, &sa, &res);
}

extern "C" void __interflop_double_v2_float_cast(v2double a,
                                                 v2OpaqueShadow256 sa,
                                                 v2OpaqueShadow128 res) {
  Backend<v2double> Backend;
  Backend.DownCast(a, sa, res);
}

// ===================
// Unary operator
// ===================

extern "C" float __interflop_float_neg(float a, OpaqueShadow128 *sa,
                                       OpaqueShadow128 *res) {
  Backend<float> Backend;
  return Backend.Neg(a, &sa, &res);
}

extern "C" double __interflop_double_neg(double a, OpaqueShadow256 *sa,
                                         OpaqueShadow256 *res) {
  Backend<double> Backend;
  return Backend.Neg(a, &sa, &res);
}

extern "C" v2double __interflop_double_v2_neg(v2double a, v2OpaqueShadow256 sa,
                                              v2OpaqueShadow256 res) {
  Backend<v2double> Backend;
  return Backend.Neg(a, sa, res);
}

// ================================
// float binary  operation overload
// ================================

extern "C" float __interflop_float_fadd(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  Backend<float> Backend;
  return Backend.Add(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fsub(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  Backend<float> Backend;
  return Backend.Sub(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fmul(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  Backend<float> Backend;
  return Backend.Mul(a, &sa, b, &sb, &res);
}

extern "C" float __interflop_float_fdiv(float a, OpaqueShadow128 *sa, float b,
                                        OpaqueShadow128 *sb,
                                        OpaqueShadow128 *res) {
  Backend<float> Backend;
  return Backend.Div(a, &sa, b, &sb, &res);
}

// =====================================
// floatbinary vector operation overload
// =====================================

extern "C" v2float __interflop_float_v2_fadd(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  Backend<v2float> Backend;
  return Backend.Add(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fsub(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  Backend<v2float> Backend;
  return Backend.Sub(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fmul(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  Backend<v2float> Backend;
  return Backend.Mul(a, sa, b, sb, res);
}

extern "C" v2float __interflop_float_v2_fdiv(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb,
                                             v2OpaqueShadow128 res) {
  Backend<v2float> Backend;
  return Backend.Div(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fadd(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  Backend<v4float> Backend;
  return Backend.Add(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fsub(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  Backend<v4float> Backend;
  return Backend.Sub(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fmul(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  Backend<v4float> Backend;
  return Backend.Mul(a, sa, b, sb, res);
}

extern "C" v4float __interflop_float_v4_fdiv(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb,
                                             v4OpaqueShadow128 res) {
  Backend<v4float> Backend;
  return Backend.Div(a, sa, b, sb, res);
}

// ================================
// double binary operation overload
// ================================

extern "C" double __interflop_double_fadd(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  Backend<double> Backend;
  return Backend.Add(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fsub(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  Backend<double> Backend;
  return Backend.Sub(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fmul(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  Backend<double> Backend;
  return Backend.Mul(a, &sa, b, &sb, &res);
}

extern "C" double __interflop_double_fdiv(double a, OpaqueShadow256 *sa,
                                          double b, OpaqueShadow256 *sb,
                                          OpaqueShadow256 *res) {
  Backend<double> Backend;
  return Backend.Div(a, &sa, b, &sb, &res);
}

// =======================================
// double binary vector operation overload
// =======================================

extern "C" v2double __interflop_double_v2_fadd(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  Backend<v2double> Backend;
  return Backend.Add(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fsub(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  Backend<v2double> Backend;
  return Backend.Sub(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fmul(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  Backend<v2double> Backend;
  return Backend.Mul(a, sa, b, sb, res);
}

extern "C" v2double __interflop_double_v2_fdiv(v2double a, v2OpaqueShadow256 sa,
                                               v2double b, v2OpaqueShadow256 sb,
                                               v2OpaqueShadow256 res) {
  Backend<v2double> Backend;
  return Backend.Div(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fadd(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  Backend<v4double> Backend;
  return Backend.Add(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fsub(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  Backend<v4double> Backend;
  return Backend.Sub(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fmul(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  Backend<v4double> Backend;
  return Backend.Mul(a, sa, b, sb, res);
}

extern "C" v4double __interflop_double_v4_fdiv(v4double a, v4OpaqueShadow256 sa,
                                               v4double b, v4OpaqueShadow256 sb,
                                               v4OpaqueShadow256 res) {
  Backend<v4double> Backend;
  return Backend.Div(a, sa, b, sb, res);
}

extern "C" int __interflop_float_check(float a, OpaqueShadow128 *sa) {
  Backend<float> Backend;
  return Backend.Check(a, &sa);
}

extern "C" int __interflop_double_check(double a, OpaqueShadow256 *sa) {
  Backend<double> Backend;
  return Backend.Check(a, &sa);
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
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, &sa, b, &sb, a == b);
}

extern "C" int __interflop_float_fcmp_one(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_one, a, &sa, b, &sb, a != b);
}

extern "C" int __interflop_float_fcmp_ogt(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_ogt, a, &sa, b, &sb, a > b);
}

extern "C" int __interflop_float_fcmp_olt(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_olt, a, &sa, b, &sb, a < b);
}

//
//
// Float Vector Comparisons
//
//

extern "C" int __interflop_float_v2_fcmp_oeq(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  Backend<v2float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2float>(a == b));
}

extern "C" int __interflop_float_v2_fcmp_one(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  Backend<v2float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2float>(a != b));
}

extern "C" int __interflop_float_v2_fcmp_ogt(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  Backend<v2float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2float>(a > b));
}

extern "C" int __interflop_float_v2_fcmp_olt(v2float a, v2OpaqueShadow128 sa,
                                             v2float b, v2OpaqueShadow128 sb) {
  Backend<v2float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2float>(a < b));
}

extern "C" int __interflop_float_v4_fcmp_oeq(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  Backend<v4float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4float>(a == b));
}

extern "C" int __interflop_float_v4_fcmp_one(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  Backend<v4float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4float>(a != b));
}

extern "C" int __interflop_float_v4_fcmp_ogt(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  Backend<v4float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4float>(a > b));
}
extern "C" int __interflop_float_v4_fcmp_olt(v4float a, v4OpaqueShadow128 sa,
                                             v4float b, v4OpaqueShadow128 sb) {
  Backend<v4float> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4float>(a < b));
}

//
//
// Double Vector Comparisons
//
//

extern "C" int __interflop_double_fcmp_oeq(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, &sa, b, &sb, a == b);
}

extern "C" int __interflop_double_fcmp_one(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_one, a, &sa, b, &sb, a != b);
}

extern "C" int __interflop_double_fcmp_ogt(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_ogt, a, &sa, b, &sb, a > b);
}

extern "C" int __interflop_double_fcmp_olt(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_olt, a, &sa, b, &sb, a < b);
}

//
//
// Double Vector Comparisons
//
//

extern "C" int __interflop_double_v2_fcmp_oeq(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  Backend<v2double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2double>(a == b));
}

extern "C" int __interflop_double_v2_fcmp_one(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  Backend<v2double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2double>(a != b));
}

extern "C" int __interflop_double_v2_fcmp_ogt(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  Backend<v2double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2double>(a > b));
}

extern "C" int __interflop_double_v2_fcmp_olt(v2double a, v2OpaqueShadow256 sa,
                                              v2double b,
                                              v2OpaqueShadow256 sb) {
  Backend<v2double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v2double>(a < b));
}

extern "C" int __interflop_double_v4_fcmp_oeq(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  Backend<v4double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4double>(a == b));
}

extern "C" int __interflop_double_v4_fcmp_one(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  Backend<v4double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4double>(a != b));
}

extern "C" int __interflop_double_v4_fcmp_ogt(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  Backend<v4double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4double>(a > b));
}
extern "C" int __interflop_double_v4_fcmp_olt(v4double a, v4OpaqueShadow256 sa,
                                              v4double b,
                                              v4OpaqueShadow256 sb) {
  Backend<v4double> Backend;
  return Backend.CheckFCmp(FCmp_oeq, a, sa, b, sb,
                           ReducePredicate<v4double>(a < b));
}

// =============================
// Unoredered Comparisons operator
// =============================

extern "C" int __interflop_float_fcmp_ueq(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_ueq, a, &sa, b, &sb, a == b);
}

extern "C" int __interflop_float_fcmp_une(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_une, a, &sa, b, &sb, a != b);
}

extern "C" int __interflop_float_fcmp_ugt(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_ugt, a, &sa, b, &sb, a > b);
}

extern "C" int __interflop_float_fcmp_ult(float a, OpaqueShadow128 *sa, float b,
                                          OpaqueShadow128 *sb) {
  Backend<float> Backend;
  return Backend.CheckFCmp(FCmp_ult, a, &sa, b, &sb, a < b);
}

extern "C" int __interflop_double_fcmp_ueq(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_ueq, a, &sa, b, &sb, a == b);
}

extern "C" int __interflop_double_fcmp_une(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_une, a, &sa, b, &sb, a != b);
}

extern "C" int __interflop_double_fcmp_ugt(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_ugt, a, &sa, b, &sb, a > b);
}

extern "C" int __interflop_double_fcmp_ult(double a, OpaqueShadow256 *sa,
                                           double b, OpaqueShadow256 *sb) {
  Backend<double> Backend;
  return Backend.CheckFCmp(FCmp_ult, a, &sa, b, &sb, a < b);
}