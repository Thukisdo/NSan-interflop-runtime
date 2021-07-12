#pragma once
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "Backend.hpp"
#include "Flags.hpp"
#include "Shadow.hpp"
#include "Utils.hpp"

namespace doubleprec {

struct DoublePrecShadow128 {
  double val;
  uint64_t padding[1];
};

struct DoublePrecShadow256 {
  __float128 val;
  uint64_t padding[2];
};

template <typename ShadowTy> struct DoublePrecShadowTy {
  using Type =
      typename std::conditional<std::is_same<ShadowTy, OpaqueShadow128>::value,
                                MCASyncShadow128, MCASyncShadow256>::type;
};

// Double precision runtime using c++ templates
// Scalar and vectors are treated in the same methods
template <typename FPType> class DoublePrecRuntime : InterflopBackendBase {
public:
  using ScalarVT = typename FPTypeInfo<FPType>::scalar_type;
  using ShadowType = typename FPTypeInfo<FPType>::type;
  using ShadowPtr = ShadowType **;
  using DoublePrecShadow = typename DoublePrecShadowTy<ShadowType>::Type;
  static constexpr size_t VectorSize = FPTypeInfo<FPType>::VectorSize;

  // Binary operator overload
  virtual FPType Add(FPType LeftOperand, ShadowPtr LeftShadowOperand,
                     FPType RightOperand, ShadowPtr const RightShadowOperand,
                     ShadowPtr Res) {

    DoublePrecShadow **LeftShadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow **RightShadow = reinterpret_cast<DoublePrecShadow **>(sb);
    DoublePrecShaodw **ResShadow = reinterprect_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val + RightShadow[I]->val;
    return a + b;
  }

  virtual FPType Sub(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                     ShadowPtr res) {
    DoublePrecShadow **LeftShadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow **RightShadow = reinterpret_cast<DoublePrecShadow **>(sb);
    DoublePrecShaodw **ResShadow = reinterprect_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val - RightShadow[I]->val;
    return a - b;
  }

  virtual FPType Mul(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                     ShadowPtr res) {
    DoublePrecShadow **LeftShadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow **RightShadow = reinterpret_cast<DoublePrecShadow **>(sb);
    DoublePrecShaodw **ResShadow = reinterprect_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val * RightShadow[I]->val;
    return a * b;
  }

  virtual FPType Div(FPType a, ShadowPtr sa, FPType b, ShadowPtr sb,
                     ShadowPtr res) {

    DoublePrecShadow **LeftShadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow **RightShadow = reinterpret_cast<DoublePrecShadow **>(sb);
    DoublePrecShadow **ResShadow = reinterprect_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val / RightShadow[I]->val;
    return a / b;
  }

  /*  virtual bool FCmp(FCmpOpcode Opcode, FPType a, ShadowPtr sa, FPType b,
                     ShadowPtr sb) {

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
   } */

  virtual bool CheckFCmp(FCmpOpcode Opcode, FPType a, ShadowPtr sa, FPType b,
                         ShadowPtr sb) {
    return true;
    /* bool res = FCmp<Opcode>(sa, sb);
    // We expect both comparison to be equal, else we print an error
    if (not RuntimeFlags::DisableWarning && c != res)
      FCmpCheckFail(a, sa, b, sb);
    // We return the shadow comparison result to be able to correctly branch
    return res; */
  }

  virtual void DownCast(FPType Operand, ShadowType **ShadowOperand,
                        OpaqueShadow128 **Res) {
    DoublePrecShadow **Shadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow128 **ResShadow =
        reinterprect_cast<DoublePrecShadow128 **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val = Shadow[I]->val;
    }
  }

  virtual void UpCast(FPType Operand, ShadowType **ShadowOperand,
                      OpaqueShadow256 **Res) {
    DoublePrecShadow **Shadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow256 **ResShadow =
        reinterprect_cast<DoublePrecShadow256 **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val = Shadow[I]->val;
    }
  }

  virtual FPType Neg(FPType Operand, ShadowPtr ShadowOperand, ShadowPtr Res) {
    DoublePrecShadow **Shadow = reinterpret_cast<DoublePrecShadow **>(sa);
    DoublePrecShadow **ResShadow = reinterprect_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val = -Shadow[I]->val;
    }
    return -Operand;
  }

  virtual void MakeShadow(FPType Operand, ShadowPtr Res) {
    DoublePrecShadow **ResShadow = reinterprect_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      // We shall only use Operand[I] when working on vectors
      if constexpr (VectorSize > 1)
        ResShadow[I]->val = Operand[I];
      else
        ResShadow[0]->val = Operand;
    }
  }

  virtual bool Check(FPType Operand, ShadowTy **ShadowOperand) {

    MCASyncShadow **Shadow = reinterpret_cast<MCASyncShadow **>(ShadowOperand);

    bool Res = 0;
    // We unvectorize the check
    // We shall not acess Operand[I] if we're not working on vectors
    if constexpr (VectorSize > 1) {
      for (int I = 0; not Res && (I < VectorSize); I++)
        Res = Res || CheckInternal(Operand[I], Shadow[I]);
      return Res;
    } else
      Res = CheckInternal(Operand, Shadow[0]);

    if (Res && not RuntimeFlags ::DisableWarning)
      CheckFail(Operand, Shadow);
    if (Res && RuntimeFlags ::ExitOnError)
      exit(1);
    return Res;
  }

private:
  /*   virtual void FCmpCheckFail(FPType a, ShadowPtr sa, FPType b,
    ShadowPtr sb) { std::cout << "floating-point comparison results depend
    on precision"
                << std::endl;
      std::cout << "\tValue a: " << a << " b: " << b << std::endl;
      std::cout << "Shadow a: { ";
      for (int I = 0; I < VectorSize; I++) {
        std::cout << sa[I]->val << " ";
      }
      std::cout << "} \nShadow b: { " << std::endl;
      for (int I = 0; I < VectorSize; I++) {
        std::cout << sb[I]->val << " ";
      }
      std::cout << "}" << std::endl;
      utils::DumpStacktrace();
      if (RuntimeFlags::ExitOnError)
        exit(1);
    } */

  bool CheckInternal(ScalarVT Operand, DoublePrecShadow *Shadow) {

    double AbsoluteError = utils::abs(Operand - Shadow->val);
    double RelativeError = AbsoluteError / Shadow->val;

    return RelativeError >= 0.001;
  }

  void CheckFail(FPType Operand, DoublePrecShadow **Shadow) {
    std::cout << "\033[1;31m";
    std::cout << "[MCASync] Inconsistent shadow result :"
              << std::setprecision(20) << std::endl;

    std::cout << "\tNative Value: ";
    // We shall not acess Operand[I] if we're not working on vectors
    if constexpr (VectorSize > 1)
      for (int I = 0; I < VectorSize; I++)
        std::cout << Operand[I] << std::endl;
    else
      std::cout << Operand << std::endl;

    std::cout << "\tShadow Value: \n\t  " << *Shadow[0] << std::endl;
    // std::cout << "SignificantDigit : " << SignificantDigit << std::endl;
    utils::DumpStacktrace();
    std::cout << "\033[0m";
  }
};
} // namespace doubleprec