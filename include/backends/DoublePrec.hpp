#pragma once
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

#include "Backend.hpp"
#include "Flags.hpp"
#include "Shadow.hpp"
#include "Utils.hpp"
namespace interflop {
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
                                DoublePrecShadow128, DoublePrecShadow256>::type;
};

// Double precision runtime using c++ templates
// Scalar and vectors are treated in the same methods
template <typename FPType>
class DoublePrecRuntime : public InterflopBackend<FPType> {
public:
  // We need aliasing to ease the backend implementation
  using ScalarVT = typename FPTypeInfo<FPType>::ScalarType;
  using ShadowType = typename FPTypeInfo<FPType>::ShadowType;
  using ShadowPtr = ShadowType **;
  using DoublePrecShadow = typename DoublePrecShadowTy<ShadowType>::Type;
  static constexpr size_t VectorSize = FPTypeInfo<FPType>::VectorSize;

  DoublePrecRuntime(RuntimeStats *Stats) : InterflopBackend<FPType>(Stats) {}
  virtual ~DoublePrecRuntime() = default;

  // FIXME : The name should be stored elsewhere in order not to require on a
  // templated object
  virtual const char *getName() const final {
    return "Double precision (adapted from NSan)";
  }

  // Binary operator overload
  // We perform the same operation in the shadow space
  virtual FPType Add(FPType LeftOperand, ShadowPtr LeftShadowOperand,
                     FPType RightOperand, ShadowPtr const RightShadowOperand,
                     ShadowPtr Res) final {

    auto LeftShadow = reinterpret_cast<DoublePrecShadow **>(LeftShadowOperand);
    auto RightShadow =
        reinterpret_cast<DoublePrecShadow **>(RightShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val + RightShadow[I]->val;
    return LeftOperand + RightOperand;
  }

  virtual FPType Sub(FPType LeftOperand, ShadowPtr LeftShadowOperand,
                     FPType RightOperand, ShadowPtr const RightShadowOperand,
                     ShadowPtr Res) final {
    auto LeftShadow = reinterpret_cast<DoublePrecShadow **>(LeftShadowOperand);
    auto RightShadow =
        reinterpret_cast<DoublePrecShadow **>(RightShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val - RightShadow[I]->val;
    return LeftOperand - RightOperand;
  }

  virtual FPType Mul(FPType LeftOperand, ShadowPtr LeftShadowOperand,
                     FPType RightOperand, ShadowPtr const RightShadowOperand,
                     ShadowPtr Res) final {
    auto LeftShadow = reinterpret_cast<DoublePrecShadow **>(LeftShadowOperand);
    auto RightShadow =
        reinterpret_cast<DoublePrecShadow **>(RightShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val * RightShadow[I]->val;
    return LeftOperand * RightOperand;
  }

  virtual FPType Div(FPType LeftOperand, ShadowPtr LeftShadowOperand,
                     FPType RightOperand, ShadowPtr const RightShadowOperand,
                     ShadowPtr Res) final {

    auto LeftShadow = reinterpret_cast<DoublePrecShadow **>(LeftShadowOperand);
    auto RightShadow =
        reinterpret_cast<DoublePrecShadow **>(RightShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++)
      ResShadow[I]->val = LeftShadow[I]->val / RightShadow[I]->val;
    return LeftOperand / RightOperand;
  }

  // We need to perform the comparison with both shadows, and compare it to the
  // native result To ease the implementaiton, we take the native result as a
  // parameter
  virtual bool CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                         ShadowPtr LeftShadowOperand, FPType RightOperand,
                         ShadowPtr RightShadowOperand, bool Value) final {

    auto LeftShadow = reinterpret_cast<DoublePrecShadow **>(LeftShadowOperand);
    auto RightShadow =
        reinterpret_cast<DoublePrecShadow **>(RightShadowOperand);

    // We perfom the same comparisons in the shadow space
    // We should take care of unordered comparisons
    bool Res = FCmp(Opcode, LeftShadow, RightShadow);

    // We expect both comparison to be equal, else we emit a warning
    if (Value != Res) {
      // We may want to store additional information
      InterflopBackend<FPType>::Stats->RegisterWarning(RuntimeStats::FCmpCheck);
      if (not RuntimeFlags::DisableWarning)
        FCmpCheckFail(LeftOperand, LeftShadow, RightOperand, RightShadow);
    }
    // We return the shadow comparison result to be able to correctly branch
    return Res;
  }

  // An issue arise regarding the destination type
  // We could use a template, but that would break polymorphism
  // Instead, we declare two function for either up/down casting
  virtual void DownCast(FPType Operand, ShadowType **ShadowOperand,
                        OpaqueShadow128 **Res) final {
    auto Shadow = reinterpret_cast<DoublePrecShadow **>(ShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow128 **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val = Shadow[I]->val;
    }
  }

  virtual void UpCast(FPType Operand, ShadowType **ShadowOperand,
                      OpaqueShadow256 **Res) final {
    auto Shadow = reinterpret_cast<DoublePrecShadow **>(ShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow256 **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val = Shadow[I]->val;
    }
  }

  virtual FPType Neg(FPType Operand, ShadowPtr ShadowOperand,
                     ShadowPtr Res) final {
    auto Shadow = reinterpret_cast<DoublePrecShadow **>(ShadowOperand);
    auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val = -Shadow[I]->val;
    }
    return -Operand;
  }

  // We simply extend the original shadow to double precision
  virtual void MakeShadow(FPType Operand, ShadowPtr Res) final {
    auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      // We shall only use Operand[I] when working on vectors
      if constexpr (VectorSize > 1)
        ResShadow[I]->val = Operand[I];
      else
        ResShadow[0]->val = Operand;
    }
  }

  // Called when we need to compare the native value with the shadow one to see
  // if they have diverged
  virtual bool Check(FPType Operand, ShadowType **ShadowOperand) final {

    DoublePrecShadow **Shadow =
        reinterpret_cast<DoublePrecShadow **>(ShadowOperand);

    bool Res = 0;
    // We unvectorize the check
    // We shall not acess Operand[I] if we're not working on vectors
    if constexpr (VectorSize > 1) {
      // FIXME : Should we check every value of the vector, or print as soon as
      // an error arises ?
      for (int I = 0; not Res && (I < VectorSize); I++)
        Res = Res || CheckInternal(Operand[I], Shadow[I]);
      return Res;
    } else
      Res = CheckInternal(Operand, Shadow[0]);

    if (Res) {
      // We may want to store additional information
      InterflopBackend<FPType>::Stats->RegisterWarning(RuntimeStats::Check);
      if (not RuntimeFlags::DisableWarning)
        CheckFail(Operand, Shadow);
      if (RuntimeFlags::ExitOnError)
        exit(1);
    }
    return Res;
  }

private:
  bool FCmp(FCmpOpcode Opcode, DoublePrecShadow **LeftShadow,
            DoublePrecShadow **RightShadow) {

    bool Res = true;

    for (int I = 0; Res && (I < VectorSize); I++) {

      auto LeftOp = LeftShadow[I]->val;
      auto RightOp = RightShadow[I]->val;

      // Handle unordered comparisons
      if (Opcode < UnorderedFCmp &&
          (utils::isnan(LeftOp) || utils::isnan(RightOp)))
        continue; // NaN <=> NaN is always true, no need to go further

      // Handle (ordered) comparisons
      // FIXME : we could gain some performance by using a constexpr if, but
      // that would require the use of a template
      if (Opcode == FCmp_oeq || Opcode == FCmp_ueq)
        Res = Res && (LeftOp == RightOp);
      else if (Opcode == FCmp_one || Opcode == FCmp_une)
        Res = Res && (LeftOp != RightOp);
      else if (Opcode == FCmp_ogt || Opcode == FCmp_ugt)
        Res = Res && (LeftOp > RightOp);
      else if (Opcode == FCmp_olt || Opcode == FCmp_ult)
        Res = Res && (LeftOp < RightOp);
      else
        utils::unreachable("Unknown Predicate");
    }
    return Res;
  }

  void FCmpCheckFail(FPType a, DoublePrecShadow **sa, FPType b,
                     DoublePrecShadow **sb) {
    std::cout << "Shadow results depends on precision" << std::endl;

    // Printing boilerplate code
    // We want to print the values and their shadow counterparts

    std::cout << "\tValue  a: { ";
    if constexpr (VectorSize > 1) {
      for (int I = 0; I < VectorSize; I++)
        std::cout << a[I] << " ";
      std::cout << "} b: ";
      for (int I = 0; I < VectorSize; I++)
        std::cout << b[I] << " ";
    } else
      std::cout << a << " } b: {" << b;

    std::cout << "}\n\tShadow a: { ";
    for (int I = 0; I < VectorSize; I++) {
      std::cout << sa[I]->val << " ";
    }
    std::cout << "} b: { ";
    for (int I = 0; I < VectorSize; I++) {
      std::cout << sb[I]->val << " ";
    }
    std::cout << "}" << std::endl;

    utils::DumpStacktrace();
    if (RuntimeFlags::ExitOnError)
      exit(1);
  }

  bool CheckInternal(ScalarVT Operand, DoublePrecShadow *Shadow) {

    // Same as nsan default max threshold
    // FIXME : Should be defined as flag for more versatility
    static constexpr double MaxAbsoluteError = 1.0 / std::pow(2, 32);
    static constexpr double MaxRelativeError = 1.0 / std::pow(2, 19);

    double AbsoluteError = utils::abs(Operand - Shadow->val);
    double RelativeError = utils::abs((AbsoluteError / Shadow->val) * 100);

    return AbsoluteError >= MaxAbsoluteError ||
           RelativeError >= MaxRelativeError;
  }

  void CheckFail(FPType Operand, DoublePrecShadow **Shadow) {

    // Boilerplate printing code

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

    std::cout << "\tShadow Value: \n\t  " << Shadow[0]->val << std::endl;
    utils::DumpStacktrace();
    std::cout << "\033[0m";
  }
};
} // namespace doubleprec
} // namespace interflop