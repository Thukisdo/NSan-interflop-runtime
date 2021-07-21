/**
 * @file Context.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Interflop context, stores the current backend and additional runtime
 *  information.
 * @version 0.5.0
 * @date 2021-07-20
 *
 */

#pragma once
#include "Backend.hpp"
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include "backends/DoublePrec.hpp"
#include "backends/MCASync.hpp"
#include <iostream>
#include <memory>

namespace interflop {

class InterflopContext {
public:
  // Pre-allocates every backend and the Stats object
  InterflopContext();

  // Since The context is a global object, it will be destroyed when the program
  // ends
  ~InterflopContext() {}

  std::string const &getBackendName() const;

  RuntimeStats *getStats() { return Stats.get(); }

  RuntimeStats const *getStats() const { return getStats(); }

  template <typename T> InterflopBackend<T> &getBackendFor() {
    // Every backend should've been created in the constructor
    // and std::unique_ptr will return nullptr thus segfaulting if it's not
    return *reinterpret_cast<InterflopBackend<T> *>(
        Backends[FPTypeInfo<T>::Type].get());
  }

private:
  // this function is marked noexcept since we should immediatly terminate the
  // program if we fail to allocate a runtime
  template <typename T>
  std::unique_ptr<InterflopBackendBase> makeBackend() noexcept {
    return std::make_unique<doubleprec::DoublePrecRuntime<T>>(Stats.get());
  }

  // We will need a runtime for each floating-point type
  // since we're not using static classes
  std::array<std::unique_ptr<InterflopBackendBase>, utils::kNumFloatType>
      Backends;
  std::unique_ptr<RuntimeStats> Stats;
};

// We should probably use a singleton for the context
// Instead of a global object, but it's easier to access this way
extern InterflopContext Context;

} // namespace interflop
