/**
 * @file Interflop.cpp
 * @author Mathys JAM (mmathys.jam@ens.uvsq.fr)
 * @brief Module constructor and startup logic.
 * @version 0.5.0
 * @date 2021-07-20
 *
 *
 */

#include "Backend.hpp"
#include "Context.hpp"
#include <iomanip>
#include <iostream>

namespace interflop {

void StacktraceRecorder::print(std::string const &BackendName,
                               std::ostream &out) {
  std::scoped_lock<std::mutex> lock(Mutex);
  // Boilerplate printing code for recorded stacktraces
  out << "\n\n";
  for (int I = 0; I < 50; ++I)
    out << "_";

  out << "\nInterflop results:"
      << "\n";
  out << "\tBackend: " << BackendName << "\n";
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

} // namespace interflop

using namespace interflop;
// Warning : since we're in a module constructor, some objects may still be
// uninitialized, like std::cout
extern "C" void __interflop_init() {
  BackendInit();
  InterflopContext::getInstance().Init();
}
