/**
 * @file Interflop.cpp
 * @author Mathys JAM (mmathys.jam@ens.uvsq.fr)
 * @brief Module constructor and startup logic.
 * @version 0.5.1
 * @date 2021-08-31
 *
 *
 */

#include "Backend.hpp"
#include "Context.hpp"
#include <atomic>
#include <iomanip>
#include <iostream>

namespace insane {

void WarningRecorder::Record() {
  RecordImpl();
  WarningCount++;

  size_t WarningLimit = InsaneContext::getInstance().Flags().getWarningLimit();

  if (WarningLimit > 0 && WarningCount > WarningLimit) {
    std::cerr << "[INSanE] Warning limit reached, exiting\n";
    exit(1);
  }
}

void StacktraceRecorder::print(std::string const &BackendName,
                               std::ostream &out) {
  std::scoped_lock<std::mutex> lock(Mutex);
  // Boilerplate printing code for recorded stacktraces
  out << "\n\n";
  for (int I = 0; I < 50; ++I)
    out << "_";

  out << "\nInsane results:"
      << "\n";
  out << "\tRuntime: " << BackendName << "\n";
  size_t WarningCount = 0;
  for (auto const &It : Map)
    WarningCount += It.second;
  out << "\tWarning(s): " << WarningCount << "\n";
  for (int I = 0; I < 50; ++I)
    out << "_";
  out << "\n";

  if (Map.empty())
    out << "==[No warning emitted]==\n";
  out << utils::AsciiColor::Red;
  for (auto const &It : Map) {
    // We need to flush the stream before printing the stack, or the stack
    // might appear before the warning
    out << It.second << " warning(s) at " << std::flush;
    utils::PrintStackTrace(It.first);
    out << std::endl;
  }
  out << utils::AsciiColor::Reset;
}

} // namespace insane

using namespace insane;
// Warning : since we're in a module constructor, some objects may still be
// uninitialized, like std::cout
extern "C" void __interflop_init() {
  // FIXME: Should use the context initialization variable ?
  static std::atomic<bool> Initialized = false;

  if (Initialized)
    return;
  Initialized = true;

  auto &Context = InsaneContext::getInstance();
  Context.Init();
  BackendInit(Context);
}
