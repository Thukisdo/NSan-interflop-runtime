/**
 * @file Context.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Interflop context, stores the current backend and additional runtime
 *  information.
 * @version 0.5.1
 * @date 2021-08-31
 *
 */

#pragma once
#include "Backend.hpp"
#include "Flags.hpp"
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <thread>

namespace insane {

// Global context object that store runtime flags and stack recorder
class InsaneContext {
public:
  /**
   * @brief Singleton accessor
   *
   * Global object construction oder is undefined between compilation unit.
   * Using a singleton declared inside a static function ensure the context
   * cannot be used before construction.
   *
   * @return InsaneContext&
   */
  static InsaneContext &getInstance();

  /**
   * @brief Deleted copy ctor to ensure singleton state
   *
   */
  InsaneContext(InsaneContext const &other) = delete;

  /**
   * @brief Deleted copy operator to ensure singleton state
   *
   */
  InsaneContext &operator=(InsaneContext const &other) = delete;

  /**
   * @brief Initialization of the context, includes flags loading.
   * Thread safe.
   *
   */
  void Init();

  /**
   * @brief Destroy the Context at the end of the program since it is declared
   * static
   *
   */
  ~InsaneContext();

  WarningRecorder &getWarningRecorder() { return *WRecorder; }

  /**
   * @brief Thread safe
   *
   * @return std::string const&
   */
  std::string const &getBackendName() {
    std::shared_lock<std::shared_mutex> lock(MainContextMutex);
    return BackendName;
  }

  /**
   * @brief Thread safe
   *
   * @return std::string const&
   */
  void setBackendName(std::string const &value) {
    std::scoped_lock<std::shared_mutex> lock(MainContextMutex);
    BackendName = value;
  }

  /**
   * @brief Returns the context flag object loaded during the initialization
   * phase
   *
   * @return RuntimeFlags&
   */
  RuntimeFlags &Flags() { return RTFlags; }

private:
  /**
   * @brief Private constructor to ensure singleton state
   *
   */
  InsaneContext() = default;

  bool Initialized{false};
  std::string BackendName{"Undefined_Backend"};

  RuntimeFlags RTFlags;
  std::unique_ptr<WarningRecorder> WRecorder;

  std::shared_mutex MainContextMutex;
};

} // namespace insane
