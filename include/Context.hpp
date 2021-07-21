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
#include <iostream>
#include <memory>

namespace interflop {

class InterflopContext {
public:
  template <typename T> using Backend = InterflopBackend<T>;

  InterflopContext();

  // Since The context is a global object, it will be destroyed when the program
  // ends
  ~InterflopContext() {}

  std::string const &getBackendName() const;

  RuntimeStats *getStats() { return Stats.get(); }

  RuntimeStats const *getStats() const { return getStats(); }

private:
  std::unique_ptr<RuntimeStats> Stats;
};

// We should probably use a singleton for the context
// Instead of a global object, but it's easier to access this way
extern InterflopContext Context;

} // namespace interflop
