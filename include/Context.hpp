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
  template <typename T> using Backend = InterflopBackend<T>;

  static InterflopContext &getInstance() {
    static InterflopContext singleton;
    return singleton;
  }

  InterflopContext(InterflopContext const& other) = delete;

  // Since The context is a global object, it will be destroyed when the program
  // ends
  ~InterflopContext() {
    // No need to print warning's stacktrace if they are disabled
    if (not RuntimeFlags::DisableWarning)
      WarningRecorder.print(BackendName, std::cout);
    BackendFinalize();  
  }

  StacktraceRecorder &getStacktraceRecorder() { return WarningRecorder; }

  std::string const &getBackendName() const;
  void setBackendName(std::string const &value) { BackendName = value; }

private:

  InterflopContext() = default;
  std::string BackendName{"Undefined_Backend"};

  StacktraceRecorder WarningRecorder;
};

} // namespace interflop
