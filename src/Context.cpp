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

InterflopContext::~InterflopContext() {
  WarningRecorder.print(BackendName, std::cout);
  BackendFinalize();
}

} // namespace interflop