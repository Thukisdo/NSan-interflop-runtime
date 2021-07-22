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
#include <unordered_map>

namespace interflop {

enum FCmpOpcode {
  OrderedFCmp,
  FCmp_oeq,
  FCmp_one,
  FCmp_ogt,
  FCmp_olt,
  UnorderedFCmp,
  FCmp_ueq,
  FCmp_une,
  FCmp_ugt,
  FCmp_ult
};

enum class InterflopBackends { DoublePrecision, MCASynchrone };

// Guaranteed to be called before the first call to the backend.
void backend_init() noexcept;
void backend_finalize() noexcept;

class WarningRecorder {
public:
  enum class WarningType { ValueCheck, FCmp };

  void print(std::string const &BackendName, std::ostream &out) {
    out << "Interflop results:"
        << "\n";
    out << "Backend: " << BackendName << "\n";

    for (auto &It : Map) {
      // We need to flush the stream before printing the stack, or the stack
      // might appear before the warning
      out << It.second << " warnings at " << std::flush;
      utils::PrintStackTrace(It.first);
      out << std::endl;
    }
  }

  void Register() { Map[utils::SaveStackTrace()]++; }

private:
  std::unordered_map<uint32_t, int> Map;
};

// Base class for all backends
// Should be derived accordingly to define multiple tools
// The context is reponsible for allowing them at startup
template <typename FPType> class InterflopBackend {
public:
  using ScalarVT = typename FPTypeInfo<FPType>::ScalarType;
  using ShadowType = typename FPTypeInfo<FPType>::ShadowType;
  static constexpr size_t VectorSize = FPTypeInfo<FPType>::VectorSize;

  // Binary operator overload
  // Should perform the operation on both the shadow and the original value
  FPType Add(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  FPType Sub(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);
  FPType Mul(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  FPType Div(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
             ShadowType **res);

  // Should return -a and res = -sa
  FPType Neg(FPType a, ShadowType **sa, ShadowType **res);

  // This function should return true if the check raised an error
  // (i.e. The shadow value is not equal to the value)
  // This will cause the programm to resume computation from the original value
  bool Check(FPType a, ShadowType **sa);

  bool CheckFCmp(FCmpOpcode Opcode, FPType LeftOperand,
                 ShadowType **LeftShadowOperand, FPType RightOperand,
                 ShadowType **RightShadowOperand, bool Value);

  void DownCast(FPType a, ShadowType **sa, OpaqueShadow128 **res);
  void UpCast(FPType a, ShadowType **sa, OpaqueShadow256 **res);

  void MakeShadow(FPType a, ShadowType **res);
};
} // namespace interflop
