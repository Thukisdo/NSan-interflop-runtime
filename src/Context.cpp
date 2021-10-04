/**
 * @file Context.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Context implementation
 * @version 0.5.1
 * @date 2021-08-31
 *
 *
 */

#include "Context.hpp"

namespace insane {

InsaneContext &InsaneContext::getInstance() {
  static InsaneContext singleton;
  return singleton;
}

void InsaneContext::Init() {

  std::scoped_lock<std::shared_mutex> lock(MainContextMutex);

  if (Initialized)
    return;

  WRecorder = std::make_unique<StacktraceRecorder>();
  RTFlags.LoadFromEnvironnement();
  RTFlags.LoadFromFile(); // File config overrides env config

  Initialized = true;
}

InsaneContext::~InsaneContext() {

  std::scoped_lock<std::shared_mutex> lock(MainContextMutex);

  // A failed initialization may indicate a failure in the progran init sequence
  // causing std::cerr to be uninitialized.
  BackendFinalize(*this);

  if (not Initialized)
    return;
  if (WRecorder && RTFlags.getPrintStatsOnExit())
    WRecorder->print(BackendName, std::cerr);

}

} // namespace insane