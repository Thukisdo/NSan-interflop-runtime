/**
 * @file MCASync.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief First prototype for a MCA Synchrone backend
 * @version 0.7.0
 * @date 2021-07-20
 *
 *
 */
#include "backends/MCASync.hpp"
#include "Context.hpp"

namespace interflop {

// Rounding functions and shadow helper methods
namespace mcasync {
std::ostream &operator<<(std::ostream &os, MCASyncShadow const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cout << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, MCASyncLargeShadow const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cout << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

// Helper struct for type puning double -> ui64
struct Float64 {
  Float64(double f) : f64(f) {}
  Float64(uint64_t i) : i64(i) {}

  union {
    double f64;
    uint64_t i64;
  };
};

// Helper struct for type puning f128 -> ui128
struct Float128 {
  Float128(__float128 f) : f128(f) {}
  Float128(__uint128_t i) : i128(i) {}
  Float128(double f)
      : f128(f) {} // Strangely, the compiler doesn't know wether to implicitly
                   // cast a double to f128 or i128, so we need an explicit ctor

  union {
    __float128 f128;
    __uint128_t i128;
  };
};

// Adapted from a Julia rounding code
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
float StochasticRound(double x) {
  static const Float64 oneF64 = 1.0;
  static const Float64 eps_F32 =
      std::nextafter((double)std::nextafter(0.0f, 1.0f), 0.0);

  uint64_t RandomBits = utils::rand<uint64_t>();
  // subnormals are rounded with float-arithmetic for uniform stoch perturbation
  // (Magic)
  if (utils::abs(x) < std::numeric_limits<float>::min()) {
    Float64 Res(oneF64.i64 | (RandomBits >> 12));
    Res.f64 -= 1.5;
    return x + eps_F32.f64 * Res.f64;
  }

  Float64 ExtendedFP{x};
  // arithmetic bitshift and |1 to create a random integer that is in (-u/2,u/2)
  // always set last random bit to 1 to avoid the creation of -u/2
  // FIXME: 36 ?
  ExtendedFP.i64 += (RandomBits >> 35) | 1;
  return ExtendedFP.f64;
}

// FIXME: Redundant code, could use templates to only have one function
double StochasticRound(__float128 x) {
  static Float128 oneF128 = 1.0;
  static Float128 eps_F64 =
      std::nextafter((double)std::nextafter(0.0, 1.0), 0.0);
  uint128_t RandomBits = utils::rand<uint128_t>(0, MAX_UINT128);

  // subnormals are round with float-arithmetic for uniform stoch perturbation
  // (Magic)
  if (utils::abs(x) < std::numeric_limits<double>::min()) {
    Float128 Res(oneF128.i128 | (RandomBits >> 16));
    Res.f128 -= 1.5;
    return x + eps_F64.f128 * Res.f128;
  }
  Float128 ExtendedFP(x);
  // arithmetic bitshift and |1 to create a random integer that is in (-u/2,u/2)
  // always set last random bit to 1 to avoid the creation of -u/2

  ExtendedFP.i128 = (ExtendedFP.i128 + (RandomBits >> 68)) | 1;
  return ExtendedFP.f128;
}

} // namespace mcasync

using namespace mcasync;

// We we'll use the random function provided with interflop-utils
// So no need for a new random generator
void BackendInit() noexcept {
  InterflopContext &Context = InterflopContext::getInstance();
  Context.setBackendName("MCA Synchrone");
  if (utils::GetNSanShadowScale() != 4) {
    fprintf(stderr, "Warning: MCA Synchrone backend requires 4x shadow\n");
    fprintf(stderr, "Recompile using flags -mllvm -nsan-shadowscale=4\n");
    exit(1);
  }
}

void BackendFinalize() noexcept {
  // Nothing to do
}

// Shadow struct and helper methods
namespace {

template <size_t VectorSize, typename FPType, typename MCASyncShadow,
          typename DestType>
void CastInternal(FPType a, MCASyncShadow **Shadow, DestType Res) {
  // We just copy every value
  for (int I = 0; I < VectorSize; I++) {
    Res[I]->val[0] = Shadow[I]->val[0];
    Res[I]->val[1] = Shadow[I]->val[1];
    Res[I]->val[2] = Shadow[I]->val[2];
  }
}

template <typename ScalarVT, typename MCASyncShadow>
bool CheckInternal(ScalarVT Operand, MCASyncShadow *Shadow) {

  double Mean = (Shadow->val[0] + Shadow->val[1] + Shadow->val[2]) / 3.0;

  double Variance = 0;
  for (int I = 0; I < 3; I++)
    Variance += pow(Shadow->val[I] - Mean, 2.0);
  Variance /= 3.0;

  double SignificantDigit = -std::log10(utils::abs(sqrt(Variance) / Mean));
  // Should use a flag that defines required precision
  return SignificantDigit <= 7;
}

template <size_t VectorSize, typename MCASyncShadow>
bool FCmpInternal(FCmpOpcode Opcode, MCASyncShadow **LeftShadow,
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
    else if (Opcode == FCmp_one || Opcode == FCmp_une)
      Res = Res && (MeanLeftOp != MeanRightOp);
    else if (Opcode == FCmp_ogt || Opcode == FCmp_ugt)
      Res = Res && (MeanLeftOp > MeanRightOp);
    else if (Opcode == FCmp_olt || Opcode == FCmp_ult)
      Res = Res && (MeanLeftOp < MeanRightOp);
    else
      utils::unreachable("Unknown Predicate");
  }
  return Res;
}

} // namespace

// Will either be double or __float128 depending on FPType
// Used to handle casting to bigger precision
template <typename ScalarVT>
using ExtendedScalar =
    typename std::conditional<std::is_same<ScalarVT, float>::value, double,
                              __float128>::type;

// Will either be MCAShadow128 or MCAShadow64 depending on FPType
template <typename FPType>
using ShadowTypeFor = typename std::conditional<
    std::is_same<typename FPTypeInfo<FPType>::ScalarType,
                 OpaqueShadow>::value,
    MCASyncShadow, MCASyncLargeShadow>::type;

template <typename FPType>
FPType InterflopBackend<FPType>::Neg(FPType Operand, ShadowType **OperandShadow,
                                     ShadowType **Res) {

  auto Shadow = reinterpret_cast<ShadowTypeFor<FPType> **>(OperandShadow);
  auto ResShadow = reinterpret_cast<ShadowTypeFor<FPType> **>(Res);

  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val[0] = -Shadow[I]->val[0];
    ResShadow[I]->val[1] = -Shadow[I]->val[1];
    ResShadow[I]->val[2] = -Shadow[I]->val[2];
  }
  return -Operand;
}

template <typename FPType>
FPType
InterflopBackend<FPType>::Add(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<ShadowTypeFor<FPType> **>(Res);

  // Perform every add in extended precision and add a rounding noise
  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val[0] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[0] + RightShadow[I]->val[0]);
    ResShadow[I]->val[1] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[1] + RightShadow[I]->val[1]);
    ResShadow[I]->val[2] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[2] + RightShadow[I]->val[2]);
  }
  return LeftOp + RightOp;
}

template <typename FPType>
FPType
InterflopBackend<FPType>::Sub(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<ShadowTypeFor<FPType> **>(Res);

  // Perform every sub in extended precision and add a rounding noise
  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val[0] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[0] - RightShadow[I]->val[0]);
    ResShadow[I]->val[1] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[1] - RightShadow[I]->val[1]);
    ResShadow[I]->val[2] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[2] - RightShadow[I]->val[2]);
  }
  return LeftOp - RightOp;
}

template <typename FPType>
FPType
InterflopBackend<FPType>::Mul(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<ShadowTypeFor<FPType> **>(Res);

  // Perform every mul in extended precision and add a rounding noise
  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val[0] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[0] * RightShadow[I]->val[0]);
    ResShadow[I]->val[1] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[1] * RightShadow[I]->val[1]);
    ResShadow[I]->val[2] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[2] * RightShadow[I]->val[2]);
  }
  return LeftOp * RightOp;
}

template <typename FPType>
FPType
InterflopBackend<FPType>::Div(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<ShadowTypeFor<FPType> **>(Res);

  // Perform every div in extended precision and add a rounding noise
  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val[0] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[0] / RightShadow[I]->val[0]);
    ResShadow[I]->val[1] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[1] / RightShadow[I]->val[1]);
    ResShadow[I]->val[2] = StochasticRound(
        (ExtendedScalar<FPType>)LeftShadow[I]->val[2] / RightShadow[I]->val[2]);
  }
  return LeftOp / RightOp;
}

template <typename FPType>
bool InterflopBackend<FPType>::Check(FPType Operand,
                                     ShadowType **ShadowOperand) {

  auto Shadow = reinterpret_cast<ShadowTypeFor<FPType> **>(ShadowOperand);

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
    auto &Context = InterflopContext::getInstance();
    Context.getStacktraceRecorder().Record();

    // Print a warning
    if (Context.Flags().getWarningEnabled()) {
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
      utils::DumpStacktrace();
      std::cout << "\033[0m";
    }

    if (Context.Flags().getExitOnError())
      exit(1);
  }
  return Res;
}

template <typename FPType>
bool InterflopBackend<FPType>::CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                                         ShadowType **LeftShadowOperand,
                                         FPType RightOperand,
                                         ShadowType **RightShadowOperand,
                                         bool Value) {

  auto LeftShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<ShadowTypeFor<FPType> **>(RightShadowOperand);
  bool Res = FCmpInternal<VectorSize>(Opcode, LeftShadow, RightShadow);
  // We expect both comparison to be equal, else we print an error
  if (Value != Res) {
    auto &Context = InterflopContext::getInstance();
    InterflopContext::getInstance().getStacktraceRecorder().Record();

    // Print a warning
    if (Context.Flags().getWarningEnabled()) {
      std::cout << "\033[1;31m";
      std::cout
          << "[MCASync] Floating-point comparison results depend on precision"
          << std::endl;
      std::cout << "\tValue  a: { ";
      if constexpr (VectorSize > 1) {
        for (int I = 0; I < VectorSize; I++)
          std::cout << LeftOperand[I] << " ";
        std::cout << "} b: ";
        for (int I = 0; I < VectorSize; I++)
          std::cout << RightOperand[I] << " ";
      } else
        std::cout << LeftOperand << " } b: {" << RightOperand;
      std::cout << "Shadow a:\n";
      for (int I = 0; I < VectorSize; I++) {
        std::cout << "\t" << *LeftShadow[I] << "\n";
      }
      std::cout << "Shadow b:\n";
      for (int I = 0; I < VectorSize; I++) {
        std::cout << "\t" << *RightShadow[I] << "\n";
      }
      utils::DumpStacktrace();
      std::cout << "\033[0m";
    }

    if (InterflopContext::getInstance().Flags().getExitOnError())
      exit(1);
    if (Context.Flags().getExitOnError())
      exit(1);
  }
  // We return the shadow comparison result to be able to correctly branch
  return Res;
}

template <typename FPType>
void InterflopBackend<FPType>::CastToFloat(FPType Operand,
                                           ShadowType **OperandShadow,
                                           OpaqueShadow **Res) {
  auto Shadow = reinterpret_cast<ShadowTypeFor<FPType> **>(OperandShadow);
  auto Destination = reinterpret_cast<MCASyncShadow **>(Res);

  CastInternal<VectorSize>(Operand, Shadow, Destination);
}

template <typename FPType>
void InterflopBackend<FPType>::CastToDouble(FPType Operand,
                                            ShadowType **OperandShadow,
                                            OpaqueLargeShadow **Res) {
  auto Shadow = reinterpret_cast<ShadowTypeFor<FPType> **>(OperandShadow);
  auto Destination = reinterpret_cast<MCASyncLargeShadow **>(Res);

  CastInternal<VectorSize>(Operand, Shadow, Destination);
}

template <typename FPType>
void InterflopBackend<FPType>::CastToLongdouble(FPType Operand,
                                                ShadowType **OperandShadow,
                                                OpaqueLargeShadow **Res) {
  auto Shadow = reinterpret_cast<ShadowTypeFor<FPType> **>(OperandShadow);
  auto Destination = reinterpret_cast<MCASyncLargeShadow **>(Res);

  CastInternal<VectorSize>(Operand, Shadow, Destination);
}

template <typename FPType>
void InterflopBackend<FPType>::MakeShadow(FPType Source, ShadowType **Res) {

  auto ResShadow = reinterpret_cast<ShadowTypeFor<FPType> **>(Res);

  for (int I = 0; I < VectorSize; I++) {
    // We need a constexpr if to prevent the compiler from evaluating a[I]
    // if its a scalar
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

template class InterflopBackend<float>;
template class InterflopBackend<v2float>;
template class InterflopBackend<v4float>;
template class InterflopBackend<v8float>;
template class InterflopBackend<double>;
template class InterflopBackend<v2double>;
template class InterflopBackend<v4double>;
// template class InterflopBackend<long double>;
// template class InterflopBackend<v2ldouble>;

} // namespace interflop
