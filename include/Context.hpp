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
#include "Flags.hpp"
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include <iostream>
#include <memory>

namespace interflop {

class InterflopContext {
public:
  static InterflopContext &getInstance();
  InterflopContext(InterflopContext const &other) = delete;
  InterflopContext &operator=(InterflopContext const &other) = delete;

  void Init();
  // Since The context is a global object, it will be destroyed when the program
  // ends
  ~InterflopContext();
  StacktraceRecorder &getStacktraceRecorder() { return WarningRecorder; }

  std::string const &getBackendName() const;
  void setBackendName(std::string const &value) { BackendName = value; }

  RuntimeFlags &Flags() { return RTFlags; }

private:
  InterflopContext() = default;

  bool Initialized{false};
  std::string BackendName{"Undefined_Backend"};
  RuntimeFlags RTFlags;
  StacktraceRecorder WarningRecorder;
};

} // namespace interflop
