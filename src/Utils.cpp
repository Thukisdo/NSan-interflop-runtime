/**
 * @file Utils.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Utilitary functions implementation
 * @version 0.5.0
 * @date 2021-07-20
 *
 */
#include "Utils.hpp"

// Defined in nsan's runtime
void __nsan_dump_stacktrace();

// Helper method to allow storing stack location in interflop mdoe
uint32_t __nsan_save_stacktrace();

// Helper method to allow printing stack location in interflop mdoe
void __nsan_print_stacktrace(uint32_t StackId);

size_t __nsan_get_shadowscale();

// When testing, we will not link to nsan runtime, so those functions will be
// undefined We declare dummy function to allow compilation
#ifdef DUMMY_NSAN_INTERFACE

void __nsan_dump_stacktrace() {}

uint32_t __nsan_save_stacktrace() { return 0; }

void __nsan_print_stacktrace(uint32_t StackId) {}

size_t __nsan_get_shadowscale() {}

#endif

namespace interflop::utils {

[[noreturn]] void unreachable(const char *str) noexcept {
  if (str)
    std::cerr << str << std::endl;
  std::cerr << "Interflop unreachable called !\n";
  DumpStacktrace();
  exit(1);
}

[[noreturn]] void exit(int status) noexcept {
  std::cerr << "Interflop has encountered an error and must exit";
  exit(status);
}

void DumpStacktrace() noexcept {
  std::cerr << "Interflop stacktrace : \n";
  __nsan_dump_stacktrace();
}

// Save the current stacktrace and returns the save ID
uint32_t SaveStackTrace() noexcept { return __nsan_save_stacktrace(); }

// Print the stacktrace of given ID
void PrintStackTrace(uint32_t StackId) noexcept {
  __nsan_print_stacktrace(StackId);
}

size_t GetNSanShadowScale()
{
  return __nsan_get_shadowscale();
}

} // namespace interflop::utils