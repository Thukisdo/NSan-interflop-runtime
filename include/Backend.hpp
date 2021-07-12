#pragma once
#include "Shadow.hpp"
#include <iostream>
#include <unordered_map>


enum FCmpOpcode {
  OrderedFCmp,
  FCmp_oeq,
  FCmp_one,
  FCmp_ogt,
  FCmp_olt,
  // FIXME Should we handle Unordered comparisons ?
  UnorderedFCmp,
  FCmp_ueq,
  FCmp_une,
  FCmp_ugt,
  FCmp_ult
};

namespace interflop {

class RuntimeStats {
public:
  enum WarningType { Check, FCmpCheck };

  void RegisterWarning(WarningType Type) { Warnings[Type]++; }

  void print(std::string const &BackendName) {
    std::cout << "Interflop instrumentation using " << BackendName
              << " Backend :\n";
    std::cout << "Check Warning(s) : " << Warnings[Check] << std::endl;
    std::cout << "FCmpCheck Warning(s) : " << Warnings[FCmpCheck] << std::endl;
  }

private:
  std::unordered_map<WarningType, unsigned long> Warnings;
};

// Needed to be able to build an array of backend
class InterflopBackendBase {
public:
  virtual const char* getName() const { return "Interflop"; }

  /* virtual ~InterflopBackendBase() = 0; */
};

template <typename FPType>
class InterflopBackend : public InterflopBackendBase {
public:
  using ScalarVT = typename FPTypeInfo<FPType>::ScalarType;
  using ShadowType = typename FPTypeInfo<FPType>::ShadowType;
  static constexpr size_t VectorSize = FPTypeInfo<FPType>::VectorSize;

  InterflopBackend(RuntimeStats *Stats) : Stats(Stats) {}

  virtual ~InterflopBackend() = default;

  // Binary operator overload
  virtual FPType Add(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
                     ShadowType **res) = 0;

  virtual FPType Sub(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
                     ShadowType **res) = 0;
  virtual FPType Mul(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
                     ShadowType **res) = 0;

  virtual FPType Div(FPType a, ShadowType **sa, FPType b, ShadowType **sb,
                     ShadowType **res) = 0;

  virtual FPType Neg(FPType a, ShadowType **sa, ShadowType **res) = 0;

  virtual bool CheckFCmp(const FCmpOpcode Opcode, FPType a, ShadowType **sa,
                         FPType b, ShadowType **sb) = 0;

  virtual void DownCast(FPType a, ShadowType **sa, OpaqueShadow128 **res) = 0;
  virtual void UpCast(FPType a, ShadowType **sa, OpaqueShadow256 **res) = 0;

  virtual void MakeShadow(FPType a, ShadowType **res) = 0;

  virtual bool Check(FPType a, ShadowType **sa) = 0;

protected:
  RuntimeStats *Stats;
};
} // namespace interflop