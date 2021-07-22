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

  // Since The context is a global object, it will be destroyed when the program
  // ends
  ~InterflopContext() {
    if (not RuntimeFlags::DisableWarning)
      WRecord.print(BackendName, std::cout);
  }

  WarningRecorder &getWarningRecorder() { return WRecord; }

  std::string const &getBackendName() const;
  void setBackendName(std::string const &value) { BackendName = value; }

private:
  std::string BackendName{"UndefinedBackend"};

  WarningRecorder WRecord;
};

} // namespace interflop
