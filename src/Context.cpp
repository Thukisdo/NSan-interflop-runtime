/**
 * @file Context.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Context implementation
 * @version 0.5.0
 * @date 2021-07-20
 *
 *
 */

#include "Context.hpp"

namespace interflop {

InterflopContext &InterflopContext::getInstance() {
  static InterflopContext singleton;
  return singleton;
}

void InterflopContext::Init() {

  std::scoped_lock<std::shared_mutex> lock(MainContextMutex);

  if (Initialized)
    return;

  // We should perfom some initialization here

  Initialized = true;
}

InterflopContext::~InterflopContext() {

  std::scoped_lock<std::shared_mutex> lock(MainContextMutex);

  // A failed initialization may indicate a failure in the progran init sequence
  // causing std::cout to be uninitialized.
  if (Initialized) {
    WarningRecorder.print(BackendName, std::cout);

    // FIXME: should probably call BackendFinalize() even if not initialized
    // and store a global status flag to indicate the status
    BackendFinalize(*this);
  }
}

} // namespace interflop