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

void InterflopContext::Init() { BackendInit(); }

InterflopContext::~InterflopContext() {
  // If initialization failed, std::cout might not be initialized correctly so
  // we shouldn't print anything
  if (Initialized)
    WarningRecorder.print(BackendName, std::cout);
  BackendFinalize();
}

} // namespace interflop