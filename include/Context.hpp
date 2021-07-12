#pragma once
#include "Backend.hpp"
#include "Shadow.hpp"
#include "Utils.hpp"
#include "backends/MCASync.hpp"
#include "backends/DoublePrec.hpp"
#include <iostream>
#include <memory>

namespace interflop {

class InterflopContext {
public:
  InterflopContext() {

    Stats = std::make_unique<RuntimeStats>();
    RuntimeStats* StatsPtr = Stats.get();

    Backends[utils::kFloat] =
        std::make_unique<doubleprec::DoublePrecRuntime<float>>(StatsPtr);
    Backends[utils::kV2Float] =
        std::make_unique<doubleprec::DoublePrecRuntime<v2float>>(StatsPtr);
    Backends[utils::kV4Float] =
        std::make_unique<doubleprec::DoublePrecRuntime<v4float>>(StatsPtr);
    Backends[utils::kDouble] =
        std::make_unique<doubleprec::DoublePrecRuntime<double>>(StatsPtr);
    Backends[utils::kV2Double] =
        std::make_unique<doubleprec::DoublePrecRuntime<v2double>>(StatsPtr);
    Backends[utils::kV4Double] =
        std::make_unique<doubleprec::DoublePrecRuntime<v4double>>(StatsPtr);
    Backends[utils::kV8Double] =
        std::make_unique<doubleprec::DoublePrecRuntime<v8double>>(StatsPtr);
  }

  ~InterflopContext() { Stats->print(Backends[utils::kFloat]->getName()); }

  std::string const &getBackendName() const;

  RuntimeStats *getStats();
  RuntimeStats const *getStats() const;

  template <typename T> InterflopBackend<T> &getBackendFor() {
    // FIXME Should we use a dynamic_cast ?
    // Reinterpret_cast has no overhead, but is less type safe
    return *reinterpret_cast<InterflopBackend<T> *>(
        Backends[FPTypeInfo<T>::Type].get());
  }

private:
  std::unique_ptr<InterflopBackendBase> Backends[utils::kNumFPType];
  std::unique_ptr<RuntimeStats> Stats;
};

extern InterflopContext Context;

} // namespace interflop
