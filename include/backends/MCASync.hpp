#pragma once
#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

#include "Backend.hpp"
#include "Flags.hpp"
#include "Shadow.hpp"
#include "Utils.hpp"

namespace interflop {
namespace mcasync {

typedef __int128_t unsigned uint128_t;

struct MCASyncShadow128 {
  float val[3];
  uint32_t padding[1];

  // helper methods for printing / acessing one of the shadow float
  friend std::ostream &operator<<(std::ostream &os, MCASyncShadow128 const &s);

  // Here the inline is needed to prevent multiple definition on top of the
  // obvious performance reason
  inline float operator[](size_t const index) const {
    assert(index < 3);
    return val[index];
  }
};

struct MCASyncShadow256 {
  double val[3];
  uint64_t padding[1];

  // helper methods for printing / acessing one of the shadow double
  friend std::ostream &operator<<(std::ostream &os, MCASyncShadow256 const &s);

  // Here the inline is needed to prevent multiple definition on top of the
  // obvious performance reason
  inline double operator[](size_t const index) const {
    assert(index < 3);
    return val[index];
  }
};

// Adapted from a Julia rounding code
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
float StochasticRound(float x);

// Adapted from a Julia rounding code
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
double StochasticRound(double x);

template <typename ShadowTy> struct MCASyncShadowTy {
  using Type =
      typename std::conditional<std::is_same<ShadowTy, OpaqueShadow128>::value,
                                MCASyncShadow128, MCASyncShadow256>::type;
};

// Double precision runtime using c++ templates
// Scalar and vectors are treated in the same methods
template <typename FPType>
class MCASyncRuntime : public InterflopBackend<FPType> {
public:
  using ScalarVT = typename FPTypeInfo<FPType>::ScalarType;
  using ShadowTy = typename FPTypeInfo<FPType>::ShadowType;
  using MCASyncShadow = typename MCASyncShadowTy<ShadowTy>::Type;
  static constexpr size_t VectorSize = FPTypeInfo<FPType>::VectorSize;

  MCASyncRuntime(RuntimeStats *Stats) : InterflopBackend<FPType>(Stats) {}
  virtual ~MCASyncRuntime() = default;

  virtual const char *getName() const final { return "MCASync"; }

  // Binary operator overload
  virtual FPType Add(FPType LeftOp, ShadowTy **LeftOpaqueShadow, FPType RightOp,
                     ShadowTy **RightOpaqueShadow, ShadowTy **Res) final {

    auto LeftShadow = reinterpret_cast<MCASyncShadow **>(LeftOpaqueShadow);
    auto RightShadow = reinterpret_cast<MCASyncShadow **>(RightOpaqueShadow);
    auto ResShadow = reinterpret_cast<MCASyncShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val[0] =
          StochasticRound(LeftShadow[I]->val[0] + RightShadow[I]->val[0]);
      ResShadow[I]->val[1] =
          StochasticRound(LeftShadow[I]->val[1] + RightShadow[I]->val[1]);
      ResShadow[I]->val[2] =
          StochasticRound(LeftShadow[I]->val[2] + RightShadow[I]->val[2]);
    }
    return LeftOp + RightOp;
  }

  virtual FPType Sub(FPType LeftOp, ShadowTy **LeftOpaqueShadow, FPType RightOp,
                     ShadowTy **RightOpaqueShadow, ShadowTy **Res) final {

    auto LeftShadow = reinterpret_cast<MCASyncShadow **>(LeftOpaqueShadow);
    auto RightShadow = reinterpret_cast<MCASyncShadow **>(RightOpaqueShadow);
    auto ResShadow = reinterpret_cast<MCASyncShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val[0] =
          StochasticRound(LeftShadow[I]->val[0] - RightShadow[I]->val[0]);
      ResShadow[I]->val[1] =
          StochasticRound(LeftShadow[I]->val[1] - RightShadow[I]->val[1]);
      ResShadow[I]->val[2] =
          StochasticRound(LeftShadow[I]->val[2] - RightShadow[I]->val[2]);
    }
    return LeftOp - RightOp;
  }

  virtual FPType Mul(FPType LeftOp, ShadowTy **LeftOpaqueShadow, FPType RightOp,
                     ShadowTy **RightOpaqueShadow, ShadowTy **Res) final {

    auto LeftShadow = reinterpret_cast<MCASyncShadow **>(LeftOpaqueShadow);
    auto RightShadow = reinterpret_cast<MCASyncShadow **>(RightOpaqueShadow);
    auto ResShadow = reinterpret_cast<MCASyncShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val[0] =
          StochasticRound(LeftShadow[I]->val[0] * RightShadow[I]->val[0]);
      ResShadow[I]->val[1] =
          StochasticRound(LeftShadow[I]->val[1] * RightShadow[I]->val[1]);
      ResShadow[I]->val[2] =
          StochasticRound(LeftShadow[I]->val[2] * RightShadow[I]->val[2]);
    }
    return LeftOp * RightOp;
  }

  virtual FPType Div(FPType LeftOp, ShadowTy **LeftOpaqueShadow, FPType RightOp,
                     ShadowTy **RightOpaqueShadow, ShadowTy **Res) final {

    auto LeftShadow = reinterpret_cast<MCASyncShadow **>(LeftOpaqueShadow);
    auto RightShadow = reinterpret_cast<MCASyncShadow **>(RightOpaqueShadow);
    auto ResShadow = reinterpret_cast<MCASyncShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val[0] =
          StochasticRound(LeftShadow[I]->val[0] / RightShadow[I]->val[0]);
      ResShadow[I]->val[1] =
          StochasticRound(LeftShadow[I]->val[1] / RightShadow[I]->val[1]);
      ResShadow[I]->val[2] =
          StochasticRound(LeftShadow[I]->val[2] / RightShadow[I]->val[2]);
    }
    return LeftOp / RightOp;
  }

  bool FCmp(FCmpOpcode Opcode, MCASyncShadow **LeftShadow,
            MCASyncShadow **RightShadow) {

    bool Res = true;

    for (int I = 0; Res && (I < VectorSize); I++) {

      double MeanLeftOp = (LeftShadow[I]->val[0] + LeftShadow[I]->val[1] +
                           LeftShadow[I]->val[2]) /
                          3;
      double MeanRightOp = (RightShadow[I]->val[0] + RightShadow[I]->val[1] +
                            RightShadow[I]->val[2]) /
                           3;

      // Handle unordered comparisons
      if (Opcode < UnorderedFCmp &&
          (utils::isnan(MeanLeftOp) || utils::isnan(MeanRightOp)))
        continue;

      // Handle (ordered) comparisons
      if (Opcode == FCmp_oeq || Opcode == FCmp_ueq)
        Res = Res && (MeanLeftOp == MeanRightOp);
      else if (Opcode == FCmp_one || Opcode == FCmp_one)
        Res = Res && (MeanLeftOp != MeanRightOp);
      else if (Opcode == FCmp_ogt || Opcode == FCmp_ogt)
        Res = Res && (MeanLeftOp > MeanRightOp);
      else if (Opcode == FCmp_olt || Opcode == FCmp_olt)
        Res = Res && (MeanLeftOp < MeanRightOp);
      else
        utils::unreachable("Unknown Predicate");
    }
    return Res;
  }

  virtual bool CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                         ShadowTy **LeftShadowOperand, FPType RightOperand,
                         ShadowTy **RightShadowOperand, bool Value) final {

    auto LeftShadow = reinterpret_cast<MCASyncShadow **>(LeftShadowOperand);
    auto RightShadow = reinterpret_cast<MCASyncShadow **>(RightShadowOperand);
    bool Res = FCmp(Opcode, LeftShadow, RightShadow);
    // We expect both comparison to be equal, else we print an error
    if (Value != Res) {
      InterflopBackend<FPType>::Stats->RegisterWarning(RuntimeStats::FCmpCheck);
      if (not RuntimeFlags::DisableWarning)
        FCmpCheckFail(LeftOperand, LeftShadow, RightOperand, RightShadow);
    }
    // We return the shadow comparison result to be able to correctly branch
    return Res;
  }

  virtual void DownCast(FPType Operand, ShadowTy **OperandShadow,
                        OpaqueShadow128 **Res) final {
    auto Shadow = reinterpret_cast<MCASyncShadow **>(OperandShadow);
    auto Destination = reinterpret_cast<MCASyncShadow128 **>(Res);

    Cast(Operand, Shadow, Destination);
  }

  virtual void UpCast(FPType Operand, ShadowTy **OperandShadow,
                      OpaqueShadow256 **Res) final {
    auto Shadow = reinterpret_cast<MCASyncShadow **>(OperandShadow);
    auto Destination = reinterpret_cast<MCASyncShadow256 **>(Res);

    Cast(Operand, Shadow, Destination);
  }

  virtual FPType Neg(FPType Operand, ShadowTy **OperandShadow,
                     ShadowTy **Res) final {

    auto Shadow = reinterpret_cast<MCASyncShadow **>(OperandShadow);
    auto ResShadow = reinterpret_cast<MCASyncShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      ResShadow[I]->val[0] = -Shadow[I]->val[0];
      ResShadow[I]->val[1] = -Shadow[I]->val[1];
      ResShadow[I]->val[2] = -Shadow[I]->val[2];
    }
    return -Operand;
  }

  virtual void MakeShadow(FPType Source, ShadowTy **Res) final {

    auto ResShadow = reinterpret_cast<MCASyncShadow **>(Res);

    for (int I = 0; I < VectorSize; I++) {
      // We need a constexpr if to prevent the compiler from evaluating a[I] if
      // its a scalar
      if constexpr (VectorSize > 1) {
        ResShadow[I]->val[0] = StochasticRound(Source[I]);
        ResShadow[I]->val[1] = StochasticRound(Source[I]);
        ResShadow[I]->val[2] = StochasticRound(Source[I]);
      } else {
        ResShadow[0]->val[0] = StochasticRound(Source);
        ResShadow[0]->val[1] = StochasticRound(Source);
        ResShadow[0]->val[2] = StochasticRound(Source);
      }
    }
  }

  virtual bool Check(FPType Operand, ShadowTy **ShadowOperand) final {

    auto Shadow = reinterpret_cast<MCASyncShadow **>(ShadowOperand);

    bool Res = 0;
    // We unvectorize the check
    // We shall not acess Operand[I] if we're not working on vectors
    if constexpr (VectorSize > 1) {
      for (int I = 0; not Res && (I < VectorSize); I++)
        Res = Res || CheckInternal(Operand[I], Shadow[I]);
      return Res;
    } else
      Res = CheckInternal(Operand, Shadow[0]);
    if (Res) {
      InterflopBackend<FPType>::Stats->RegisterWarning(RuntimeStats::Check);
      if (not RuntimeFlags::DisableWarning)
        CheckFail(Operand, Shadow);
      if (RuntimeFlags::ExitOnError)
        exit(1);
    }
    return Res;
  }

private:
  template <typename DestType>
  void Cast(FPType a, MCASyncShadow **Shadow, DestType Res) {
    for (int I = 0; I < VectorSize; I++) {
      Res[I]->val[0] = Shadow[I]->val[0];
      Res[I]->val[1] = Shadow[I]->val[1];
      Res[I]->val[2] = Shadow[I]->val[2];
    }
  }

  bool CheckInternal(ScalarVT Operand, MCASyncShadow *Shadow) {

    double Mean = (Shadow->val[0] + Shadow->val[1] + Shadow->val[2]) / 3.0;

    double Variance = 0;
    for (int I = 0; I < 3; I++)
      Variance += pow(Shadow->val[I] - Mean, 2.0);
    Variance /= 3.0;

    double SignificantDigit = -std::log10(utils::abs(sqrt(Variance) / Mean));
    return SignificantDigit <= 7;
  }

  void CheckFail(FPType Operand, MCASyncShadow **Shadow) {
    std::cout << "\033[1;31m";
    std::cout << "[MCASync] Low precision shadow result :"
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

  virtual void FCmpCheckFail(FPType a, ShadowTy **sa, FPType b, ShadowTy **sb) {

    std::cout << "\033[1;31m";
    std::cout
        << "[MCASync] Floating-point comparison results depend on precision"
        << std::endl;
    std::cout << "\tValue  a: { ";
    if constexpr (VectorSize > 1) {
      for (int I = 0; I < VectorSize; I++)
        std::cout << a[I] << " ";
      std::cout << "} b: ";
      for (int I = 0; I < VectorSize; I++)
        std::cout << b[I] << " ";
    } else
      std::cout << a << " } b: {" << b;
    std::cout << "Shadow a:\n";
    for (int I = 0; I < VectorSize; I++) {
      std::cout << "\t" << *sa[I] << "\n";
    }
    std::cout << "Shadow b:\n";
    for (int I = 0; I < VectorSize; I++) {
      std::cout << "\t" << *sb[I] << "\n";
    }
    utils::DumpStacktrace();
    std::cout << "\033[0m";
    if (RuntimeFlags::ExitOnError)
      exit(1);
  }
};

} // namespace mcasync
} // namespace interflop
