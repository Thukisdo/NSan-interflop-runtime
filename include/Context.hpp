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
  }

  ~InterflopContext() { Stats->print(""); }

  std::string const &getBackendName() const;

  RuntimeStats *getStats();
  RuntimeStats const *getStats() const;

  template <typename T> InterflopBackend<T> &getBackendFor() {
    // Lazily create backends
    if (Backends[FPTypeInfo<T>::Type] == nullptr)
        Backends[FPTypeInfo<T>::Type] = makeBackend<T>();
    return *reinterpret_cast<InterflopBackend<T> *>(
        Backends[FPTypeInfo<T>::Type].get());
  }

private:

  template<typename T>
  std::unique_ptr<InterflopBackendBase> makeBackend()
  {
    return std::make_unique<doubleprec::DoublePrecRuntime<T>>(Stats.get());
  }  

  std::array<std::unique_ptr<InterflopBackendBase>, utils::kNumFPType> Backends;
  std::unique_ptr<RuntimeStats> Stats;
};

extern InterflopContext Context;

} // namespace interflop



