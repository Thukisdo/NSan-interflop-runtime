/**
 * @file Backend.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Base classes for the backends.
 * @version 0.5.0
 * @date 2021-07-20
 *
 *
 */

#pragma once
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include <iostream>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace interflop {

enum FCmpOpcode {
  OrderedFCmp,
  FCmp_oeq,
  FCmp_one,
  FCmp_ogt,
  FCmp_oge,
  FCmp_olt,
  FCmp_ole,
  UnorderedFCmp,
  FCmp_ueq,
  FCmp_une,
  FCmp_ugt,
  FCmp_uge,
  FCmp_ult,
  FCmp_ule
};

// Forward declaration to avoid circular inclusion
class InterflopContext;

// Guaranteed to be called before the first call to the backend.
// std::cout and other std::streams should not be used in those functions
// as they are called before std:: initialization
void BackendInit(InterflopContext &Context) noexcept;
void BackendFinalize(InterflopContext &Context) noexcept;

// Helper class for recording warnings.
// Will record a stacktrace when a warning is emited, and print every
// recorded stacktrace at the end of the program. This enable precisely locating
// fp errors.
class StacktraceRecorder {
public:
  // Output to every stacktrace recorded to the given stream
  void print(std::string const &BackendName, std::ostream &out);

  // Record a warning, automatically saving the stacktrace
  // This method is thread safe.
  void Record() {
    std::scoped_lock<std::mutex> lock(Mutex);
    Map[utils::SaveStackTrace()]++;
  }

private:
  std::unordered_map<uint32_t, int> Map;
  std::mutex Mutex;
};

// Base class for all backends
// Should be derived accordingly to define multiple tools
// The context is reponsible for allowing them at startup
// FIXME: With c++20, we should use concepts for proper template usage
// maybe something like Shadowable<FPType> ?
template <typename MetaFP> class InterflopBackend {
public:
  using FPType = typename MetaFP::FPType;
  using ShadowType = typename MetaFP::ShadowType;
  static constexpr size_t VectorSize = MetaFP::VectorSize;

  // Should return -a and res = -sa
  FPType Neg(FPType a, ShadowType **sa, ShadowType **res);

  // Binary operator overload
  // Cauton: Should perform the operation on both the shadow and the original
  // values
  FPType Add(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  FPType Sub(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);
  FPType Mul(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  FPType Div(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  // Check functions should return true if the check raised an error
  // (i.e. The shadow value is not equal to the value or the comparisons failed)
  // This will cause the programm to resume computation from the original value
  bool Check(FPType a, ShadowType **sa);

  bool CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                 ShadowType **LeftShadowOperand, FPType RightOperand,
                 ShadowType **RightShadowOperand, bool Value);

  void MakeShadow(FPType a, ShadowType **res);

  void CastToFloat(FPType a, ShadowType **sa, OpaqueShadow **res);
  void CastToDouble(FPType a, ShadowType **sa, OpaqueLargeShadow **res);
  void CastToLongdouble(FPType a, ShadowType **sa, OpaqueLargeShadow **res);
};
} // namespace interflop
