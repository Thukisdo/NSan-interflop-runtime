/**
 * @file MCASync.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr), Pablo Oliveira
 * (pablo.oliveira@ens.uvsq.fr) and Eric Petit (eric.petit@ens.uvsq.fr)
 * @brief MCA Synchrone backend
 * @version 0.7.1
 * @date 2021-08-31
 *
 *
 */
#include "backends/MCASync.hpp"
#include "Context.hpp"

namespace insane {

/* ========================================================================= */
/* MCASync backend specific functions and structures                         */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* ========================================================================= */
namespace mcasync {

std::ostream &operator<<(std::ostream &os, MCASyncShadow const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cerr << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, MCASyncLargeShadow const &s) {
  auto mean = (s[0] + s[1] + s[2]) / 3;
  std::cerr << "[mean: " << mean << ", " << s[0] << ", " << s[1] << ", " << s[2]
            << "]";
  return os;
}

// Helper struct for type puning double -> i64
struct Float64 {
  Float64(double f) : f64(f) {}
  Float64(int64_t i) : i64(i) {}

  union {
    double f64;
    int64_t i64;
  };
};

// Helper struct for type puning f128 -> i128
struct Float128 {
  Float128(__float128 f) : f128(f) {}
  Float128(__int128_t i) : i128(i) {}
  Float128(double f)
      : f128(f) {} // Strangely, the compiler doesn't know wether to implicitly
                   // cast a double to f128 or i128, so we need an explicit ctor

  union {
    __float128 f128;
    __int128_t i128;
  };
};

// Adapted from a Julia rounding code
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
float StochasticRound(double x) {
  static const Float64 oneF64{1.0};
  static const Float64 eps_F32{std::nextafter(
      (double)std::nextafter(0.0f, std::numeric_limits<float>::max()),
      std::numeric_limits<double>::min())};

  if (std::isinf(x))
    return x;

  // Caution: we must not generate unsigned radom bits, because the output
  // will be biased
  int64_t RandomBits = utils::rand<int64_t>();
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
  ExtendedFP.i64 += (RandomBits >> 35) | 1;
  return ExtendedFP.f64;
}

double StochasticRound(__float128 x) {
  static Float128 oneF128 = 1.0;
  static Float128 eps_F64 = std::nextafter(
      (double)std::nextafter(0.0, std::numeric_limits<double>::max()),
      std::numeric_limits<double>::min());

  if (x == FLOAT128_INFINITY || x == -FLOAT128_INFINITY)
    return x;

  int128_t RandomBits = utils::rand<int128_t>();
  // subnormals are rounded with float-arithmetic for uniform stoch perturbation
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

/* ========================================================================= */
/* MCASync backend implementation                                            */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* ========================================================================= */

using namespace mcasync;

// We have to use printf() during the initialization
void BackendInit(InsaneContext &Context) noexcept {
  Context.setBackendName("insane::MCASync");

  if (utils::GetNSanShadowScale() != 4) {
    fprintf(stderr, "Warning: MCA Synchrone backend requires 4x shadow\n");
    fprintf(stderr, "Recompile using flags -mllvm -nsan-shadowscale=4\n");
    exit(1);
  }
}

void BackendFinalize(InsaneContext &Context) noexcept {
  // Nothing to do
}

// Shadow struct and helper methods
namespace {

template <size_t VectorSize, typename MCASyncShadow, typename DestType>
void CastInternal(MCASyncShadow **Shadow, DestType Res) {
  // We just copy every value
  for (int I = 0; I < VectorSize; I++) {
    Res[I]->val[0] = Shadow[I]->val[0];
    Res[I]->val[1] = Shadow[I]->val[1];
    Res[I]->val[2] = Shadow[I]->val[2];
  }
}

template <typename MCASyncShadow> bool CheckInternal(MCASyncShadow *Shadow) {

  double Mean = Shadow->mean();

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

    double MeanLeftOp = LeftShadow[I]->mean();
    double MeanRightOp = RightShadow[I]->mean();

    // Handle unordered comparisons
    if (Opcode < UnorderedFCmp &&
        (utils::isnan(MeanLeftOp) || utils::isnan(MeanRightOp)))
      continue;

    // Handle (ordered) comparisons
    // FIXME: Refactor this atrocity
    if (Opcode == FCmp_oeq || Opcode == FCmp_ueq)
      Res = Res && (MeanLeftOp == MeanRightOp);
    else if (Opcode == FCmp_one || Opcode == FCmp_une)
      Res = Res && (MeanLeftOp != MeanRightOp);
    else if (Opcode == FCmp_ogt || Opcode == FCmp_ugt)
      Res = Res && (MeanLeftOp > MeanRightOp);
    else if (Opcode == FCmp_oge || Opcode == FCmp_uge)
      Res = Res && (MeanLeftOp >= MeanRightOp);
    else if (Opcode == FCmp_olt || Opcode == FCmp_ult)
      Res = Res && (MeanLeftOp < MeanRightOp);
    else if (Opcode == FCmp_ole || Opcode == FCmp_ule)
      Res = Res && (MeanLeftOp >= MeanRightOp);
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
    typename std::conditional<std::is_same_v<ScalarVT, float>, double,
                              __float128>::type;

// Will either be MCAShadow128 or MCAShadow64 depending on FPType
template <typename ShadowType>
using MCASyncShadowFor =
    typename std::conditional<std::is_same_v<OpaqueShadow, ShadowType>,
                              MCASyncShadow, MCASyncLargeShadow>::type;

template <typename MetaFloat>
typename MetaFloat::FPType
InsaneRuntime<MetaFloat>::Neg(FPType Operand, ShadowType **OperandShadow,
                              ShadowType **Res) {

  auto Shadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(OperandShadow);
  auto ResShadow = reinterpret_cast<MCASyncShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val[0] = -Shadow[I]->val[0];
    ResShadow[I]->val[1] = -Shadow[I]->val[1];
    ResShadow[I]->val[2] = -Shadow[I]->val[2];
  }
  return -Operand;
}

template <typename MetaFloat>
typename MetaFloat::FPType
InsaneRuntime<MetaFloat>::Add(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<MCASyncShadowFor<ShadowType> **>(Res);

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

template <typename MetaFloat>
typename MetaFloat::FPType
InsaneRuntime<MetaFloat>::Sub(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<MCASyncShadowFor<ShadowType> **>(Res);

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

template <typename MetaFloat>
typename MetaFloat::FPType
InsaneRuntime<MetaFloat>::Mul(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<MCASyncShadowFor<ShadowType> **>(Res);

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

template <typename MetaFloat>
typename MetaFloat::FPType
InsaneRuntime<MetaFloat>::Div(FPType LeftOp, ShadowType **LeftOpaqueShadow,
                              FPType RightOp, ShadowType **RightOpaqueShadow,
                              ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(LeftOpaqueShadow);
  auto RightShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(RightOpaqueShadow);
  auto ResShadow = reinterpret_cast<MCASyncShadowFor<ShadowType> **>(Res);

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

template <typename MetaFloat>
bool InsaneRuntime<MetaFloat>::Check(FPType Operand,
                                     ShadowType **ShadowOperand) {

  auto Shadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(ShadowOperand);

  bool Res = 0;
  // We unvectorize the check
  // We shall not acess Operand[I] if we're not working on vectors
  if constexpr (VectorSize > 1) {
    for (int I = 0; not Res && (I < VectorSize); I++)
      Res = Res || CheckInternal(Shadow[I]);
    return Res;
  } else
    Res = CheckInternal(Shadow[0]);
  if (Res) {
    auto &Context = InsaneContext::getInstance();

    if (Context.Flags().getStackRecording())
      Context.getWarningRecorder().Record();

    // Print a warning
    if (Context.Flags().getWarningEnabled()) {
      std::cerr << "\033[1;31m";
      std::cerr << "[MCASync] Low precision shadow result :"
                << std::setprecision(20) << std::endl;

      std::cerr << "\tNative Value: ";
      // We shall not acess Operand[I] if we're not working on vectors
      if constexpr (VectorSize > 1)
        for (int I = 0; I < VectorSize; I++)
          std::cerr << Operand[I] << std::endl;
      else
        std::cerr << Operand << std::endl;

      std::cerr << "\tShadow Value: \n\t  " << *Shadow[0] << std::endl;
      utils::DumpStacktrace();
      std::cerr << "\033[0m";
    }

    if (Context.Flags().getExitOnError())
      exit(1);
  }
  return Res;
}

template <typename MetaFloat>
bool InsaneRuntime<MetaFloat>::CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                                         ShadowType **LeftShadowOperand,
                                         FPType RightOperand,
                                         ShadowType **RightShadowOperand,
                                         bool Value) {

  auto LeftShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(RightShadowOperand);
  bool Res = FCmpInternal<VectorSize>(Opcode, LeftShadow, RightShadow);
  // We expect both comparison to be equal, else we print an error
  if (Value != Res) {
    auto &Context = InsaneContext::getInstance();
    InsaneContext::getInstance().getWarningRecorder().Record();

    // Print a warning
    if (Context.Flags().getWarningEnabled()) {
      std::cerr << utils::AsciiColor::Red;
      std::cerr
          << "[MCASync] Floating-point comparison results depend on precision"
          << std::endl;
      std::cerr << "\tValue  a: { ";
      if constexpr (VectorSize > 1) {
        for (int I = 0; I < VectorSize; I++)
          std::cerr << LeftOperand[I] << " ";
        std::cerr << "} b: ";
        for (int I = 0; I < VectorSize; I++)
          std::cerr << RightOperand[I] << " ";
      } else
        std::cerr << LeftOperand << " } b: {" << RightOperand;
      std::cerr << "Shadow a:\n";
      for (int I = 0; I < VectorSize; I++) {
        std::cerr << "\t" << *LeftShadow[I] << "\n";
      }
      std::cerr << "Shadow b:\n";
      for (int I = 0; I < VectorSize; I++) {
        std::cerr << "\t" << *RightShadow[I] << "\n";
      }
      utils::DumpStacktrace();
      std::cerr << utils::AsciiColor::Reset;
    }

    if (InsaneContext::getInstance().Flags().getExitOnError())
      exit(1);
  }
  // We return the shadow comparison result to be able to correctly branch
  return Res;
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::CastToFloat(FPType Operand,
                                           ShadowType **OperandShadow,
                                           OpaqueShadow **Res) {
  auto Shadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(OperandShadow);
  auto Destination = reinterpret_cast<MCASyncShadow **>(Res);

  CastInternal<VectorSize>(Shadow, Destination);
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::CastToDouble(FPType Operand,
                                            ShadowType **OperandShadow,
                                            OpaqueLargeShadow **Res) {
  auto Shadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(OperandShadow);
  auto Destination = reinterpret_cast<MCASyncLargeShadow **>(Res);

  CastInternal<VectorSize>(Shadow, Destination);
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::CastToLongdouble(FPType Operand,
                                                ShadowType **OperandShadow,
                                                OpaqueLargeShadow **Res) {
  auto Shadow =
      reinterpret_cast<MCASyncShadowFor<ShadowType> **>(OperandShadow);
  auto Destination = reinterpret_cast<MCASyncLargeShadow **>(Res);

  CastInternal<VectorSize>(Shadow, Destination);
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::MakeShadow(FPType Source, ShadowType **Res) {

  auto ResShadow = reinterpret_cast<MCASyncShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++) {
    // We need a constexpr if to prevent the compiler from evaluating a[I]
    // if its a scalar
    if constexpr (VectorSize > 1) {
      ResShadow[I]->val[0] = Source[I];
      ResShadow[I]->val[1] = Source[I];
      ResShadow[I]->val[2] = Source[I];
    } else {
      ResShadow[0]->val[0] = Source;
      ResShadow[0]->val[1] = Source;
      ResShadow[0]->val[2] = Source;
    }
  }
}

template class InsaneRuntime<MetaFloat<float, 1>>;
template class InsaneRuntime<MetaFloat<float, 2>>;
template class InsaneRuntime<MetaFloat<float, 4>>;
template class InsaneRuntime<MetaFloat<float, 8>>;
template class InsaneRuntime<MetaFloat<float, 16>>;
template class InsaneRuntime<MetaFloat<float, 32>>;

template class InsaneRuntime<MetaFloat<double, 1>>;
template class InsaneRuntime<MetaFloat<double, 2>>;
template class InsaneRuntime<MetaFloat<double, 4>>;
template class InsaneRuntime<MetaFloat<double, 8>>;
template class InsaneRuntime<MetaFloat<double, 16>>;

template class InsaneRuntime<MetaFloat<long double, 1>>;
template class InsaneRuntime<MetaFloat<long double, 2>>;
template class InsaneRuntime<MetaFloat<long double, 4>>;
template class InsaneRuntime<MetaFloat<long double, 8>>;

} // namespace insane
