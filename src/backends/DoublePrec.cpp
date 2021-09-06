/**
 * @file DoublePrec.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Double precision backend that mimic normal nsan behaviour
 * @version 9.1.0
 * @date 2021-08-31
 *
 *
 */

#include "backends/DoublePrec.hpp"
#include "Context.hpp"
#include <cstring>

namespace insane {

using namespace doubleprec;

// We have to use printf() during the initialization
void BackendInit(InsaneContext &Context) noexcept {
  Context.setBackendName("insane::DoublePrec");

  if (utils::GetNSanShadowScale() != 2) {
    fprintf(stderr, "Warning: [DoublePrec] backend requires 2x shadow\n");
    fprintf(stderr, "Recompile using flags -mllvm -nsan-shadowscale=2\n");
    exit(1);
  }
}

void BackendFinalize(InsaneContext &Context) noexcept {
  // Nothing to do
}

namespace {

// Helper method to perform FP comparisons
// FIXME : Doesn't raise the same errors as the original code
template <size_t VectorSize, typename DoublePrecShadow>
bool FCmp(FCmpOpcode Opcode, DoublePrecShadow **LeftShadow,
          DoublePrecShadow **RightShadow) {

  bool Res = true;

  for (int I = 0; Res && (I < VectorSize); I++) {

    auto LeftOp = LeftShadow[I]->val;
    auto RightOp = RightShadow[I]->val;

    // Handle unordered comparisons
    if (Opcode > UnorderedFCmp &&
        (utils::isnan(LeftOp) || utils::isnan(RightOp)))
      continue; // NaN <=> NaN is always true, no need to go further

    // Handle (ordered) comparisons
    // FIXME: Refactor this atrocity
    if (Opcode == FCmp_oeq || Opcode == FCmp_ueq)
      Res = Res && (LeftOp == RightOp);
    else if (Opcode == FCmp_one || Opcode == FCmp_une)
      Res = Res && (LeftOp != RightOp);
    else if (Opcode == FCmp_ogt || Opcode == FCmp_ugt)
      Res = Res && (LeftOp > RightOp);
    else if (Opcode == FCmp_oge || Opcode == FCmp_uge)
      Res = Res && (LeftOp >= RightOp);
    else if (Opcode == FCmp_olt || Opcode == FCmp_ult)
      Res = Res && (LeftOp < RightOp);
    else if (Opcode == FCmp_ole || Opcode == FCmp_ule)
      Res = Res && (LeftOp <= RightOp);
    else
      utils::unreachable("Unknown Predicate");
  }
  return Res;
}

template <size_t VectorSize, typename FPType, typename DoublePrecShadow>
void FCmpCheckFail(FPType a, DoublePrecShadow **sa, FPType b,
                   DoublePrecShadow **sb) {

  std::cout << "Shadow results depends on precision" << std::endl;
  std::cout << "\tValue  a: { ";

  // Avoid acessing a[I] if we're not working on vectors
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
  if (InsaneContext::getInstance().Flags().getExitOnError())
    exit(1);
}

template <typename ScalarVT, typename DoublePrecShadow>
bool CheckInternal(ScalarVT Operand, DoublePrecShadow *Shadow) {

  // Same as nsan default max threshold
  // FIXME : Should be defined as flags for more versatility
  // Note: Some compilers do not allow for constexpr math function
  // so this was removed for compatibility purposes 
  static const double MaxAbsoluteError = 1.0 / std::pow(2, 32);
  static const double MaxRelativeError = 1.0 / std::pow(2, 19);

  double AbsoluteError = utils::abs(Operand - Shadow->val);
  double RelativeError = utils::abs((AbsoluteError / Shadow->val) * 100);
  return AbsoluteError >= MaxAbsoluteError || RelativeError >= MaxRelativeError;
}

template <size_t VectorSize, typename FPType, typename DoublePrecShadow>
void CheckFail(FPType Operand, DoublePrecShadow **Shadow) {

  std::cout << utils::AsciiColor::Red;
  std::cout << "[DoublePrec] Inconsistent shadow result :"
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
  std::cout << utils::AsciiColor::Reset;
}

template <size_t VectorSize, typename FPType, typename ShadowType,
          typename DestType>
void CastInternal(FPType a, ShadowType **sa, DestType **sb) {

  for (int I = 0; I < VectorSize; I++) {
    sb[I]->val = sa[I]->val;
  }
}

} // namespace

// Will either be DoubleprecShadow128 or DoubleprecShadow64 depending on FPType
template <typename ShadowType>
using DoubleprecShadowFor =
    typename std::conditional<std::is_same_v<OpaqueShadow, ShadowType>,
                              DoublePrecShadow, DoublePrecLargeShadow>::type;

// Unary operator overload
template <typename MetaFloat>
typename MetaFloat::FPType
InsaneRuntime<MetaFloat>::Neg(FPType Operand, ShadowType **ShadowOperand,
                                 ShadowType **Res) {

  auto Shadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(ShadowOperand);
  auto ResShadow = reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++) {
    ResShadow[I]->val = -Shadow[I]->val;
  }
  return -Operand;
}

// Binary operator overload
// Replicate
template <typename MetaFloat>
typename MetaFloat::FPType InsaneRuntime<MetaFloat>::Add(
    FPType LeftOperand, ShadowType **LeftShadowOperand, FPType RightOperand,
    ShadowType **const RightShadowOperand, ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(RightShadowOperand);
  auto ResShadow = reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++)
    ResShadow[I]->val = LeftShadow[I]->val + RightShadow[I]->val;
  return LeftOperand + RightOperand;
}

template <typename MetaFloat>
typename MetaFloat::FPType InsaneRuntime<MetaFloat>::Sub(
    FPType LeftOperand, ShadowType **LeftShadowOperand, FPType RightOperand,
    ShadowType **const RightShadowOperand, ShadowType **Res) {
  auto LeftShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(RightShadowOperand);
  auto ResShadow = reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++)
    ResShadow[I]->val = LeftShadow[I]->val - RightShadow[I]->val;
  return LeftOperand - RightOperand;
}

template <typename MetaFloat>
typename MetaFloat::FPType InsaneRuntime<MetaFloat>::Mul(
    FPType LeftOperand, ShadowType **LeftShadowOperand, FPType RightOperand,
    ShadowType **const RightShadowOperand, ShadowType **Res) {
  auto LeftShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(RightShadowOperand);
  auto ResShadow = reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++)
    ResShadow[I]->val = LeftShadow[I]->val * RightShadow[I]->val;
  return LeftOperand * RightOperand;
}

template <typename MetaFloat>
typename MetaFloat::FPType InsaneRuntime<MetaFloat>::Div(
    FPType LeftOperand, ShadowType **LeftShadowOperand, FPType RightOperand,
    ShadowType **const RightShadowOperand, ShadowType **Res) {

  auto LeftShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(RightShadowOperand);
  auto ResShadow = reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++)
    ResShadow[I]->val = LeftShadow[I]->val / RightShadow[I]->val;
  return LeftOperand / RightOperand;
}

// Called when we need to compare the native value with the shadow one to see
// if they have diverged
template <typename MetaFloat>
bool InsaneRuntime<MetaFloat>::Check(FPType Operand,
                                        ShadowType **ShadowOperand) {

  auto **Shadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(ShadowOperand);

  bool Res = false;
  // We unvectorize the check
  // We shall not acess Operand[I] if we're not working on vectors
  if constexpr (VectorSize > 1) {
    // Loop until failure or all elements have been checked
    for (int I = 0; not Res && (I < VectorSize); I++)
      Res = Res || CheckInternal(Operand[I], Shadow[I]);
    return Res;
  } else
    Res = CheckInternal(Operand, Shadow[0]);

  if (Res) {
    // We may want to store additional information
    auto &Context = InsaneContext::getInstance();

    if (Context.Flags().getStackRecording())
      Context.getWarningRecorder().Record();
    if (Context.Flags().getWarningEnabled())
      CheckFail<VectorSize>(Operand, Shadow);

    if (Context.Flags().getExitOnError())
      exit(1);
  }
  return Res;
}

// We need to perform the comparison with both shadows, and compare it to the
// native result
// To ease the implementaiton, we take the native result as a
// parameter
template <typename MetaFloat>
bool InsaneRuntime<MetaFloat>::CheckFCmp(
    FCmpOpcode Opcode, FPType LeftOperand, ShadowType **LeftShadowOperand,
    FPType RightOperand, ShadowType **RightShadowOperand, bool Value) {

  auto LeftShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(LeftShadowOperand);
  auto RightShadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(RightShadowOperand);

  // We perfom the same comparisons in the shadow space
  bool Res = FCmp<VectorSize>(Opcode, LeftShadow, RightShadow);

  // We expect both comparison to be equal, else we emit a warning
  if (Value != Res) {
    // We may want to store additional informations
    auto &Context = InsaneContext::getInstance();

    if (Context.Flags().getStackRecording())
      Context.getWarningRecorder().Record();
    if (Context.Flags().getWarningEnabled())
      FCmpCheckFail<VectorSize>(LeftOperand, LeftShadow, RightOperand,
                                RightShadow);

    if (Context.Flags().getExitOnError())
      exit(1);
  }
  // We return the shadow comparison result to be able to correctly branch
  return Res;
}

// We simply extend the original shadow to double precision
template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::MakeShadow(FPType Operand, ShadowType **Res) {
  auto ResShadow = reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(Res);

  for (int I = 0; I < VectorSize; I++) {
    // We shall only use Operand[I] when working on vectors
    if constexpr (VectorSize > 1)
      ResShadow[I]->val = Operand[I];
    else
      ResShadow[0]->val = Operand;
  }
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::CastToFloat(FPType Operand,
                                              ShadowType **ShadowOperand,
                                              OpaqueShadow **Res) {
  auto Shadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(ShadowOperand);
  auto ResShadow = reinterpret_cast<DoublePrecShadow **>(Res);

  CastInternal<VectorSize>(Operand, Shadow, ResShadow);
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::CastToDouble(FPType Operand,
                                               ShadowType **ShadowOperand,
                                               OpaqueLargeShadow **Res) {
  auto Shadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(ShadowOperand);
  auto ResShadow = reinterpret_cast<DoublePrecLargeShadow **>(Res);

  CastInternal<VectorSize>(Operand, Shadow, ResShadow);
}

template <typename MetaFloat>
void InsaneRuntime<MetaFloat>::CastToLongdouble(FPType Operand,
                                                   ShadowType **ShadowOperand,
                                                   OpaqueLargeShadow **Res) {
  auto Shadow =
      reinterpret_cast<DoubleprecShadowFor<ShadowType> **>(ShadowOperand);
  auto ResShadow = reinterpret_cast<DoublePrecLargeShadow **>(Res);

  CastInternal<VectorSize>(Operand, Shadow, ResShadow);
}

// Explicit instanciation
// Required since the interface has no access to the template definition
template class InsaneRuntime<MetaFloat<float, 1>>;
template class InsaneRuntime<MetaFloat<float, 2>>;
template class InsaneRuntime<MetaFloat<float, 4>>;
template class InsaneRuntime<MetaFloat<float, 8>>;
template class InsaneRuntime<MetaFloat<float, 16>>;

template class InsaneRuntime<MetaFloat<double, 1>>;
template class InsaneRuntime<MetaFloat<double, 2>>;
template class InsaneRuntime<MetaFloat<double, 4>>;
template class InsaneRuntime<MetaFloat<double, 8>>;

template class InsaneRuntime<MetaFloat<long double, 1>>;
template class InsaneRuntime<MetaFloat<long double, 2>>;
template class InsaneRuntime<MetaFloat<long double, 4>>;

} // namespace insane