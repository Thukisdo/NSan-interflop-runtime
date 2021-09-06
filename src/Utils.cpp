/**
 * @file Utils.cpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Utilitary functions implementation
 * @version 0.5.1
 * @date 2021-08-31
 *
 */
#include "Utils.hpp"
#include "Context.hpp"
#include "fstream"

// Defined in nsan's runtime
void __nsan_dump_stacktrace();

// Helper method to allow storing stack location in insane mode
uint32_t __nsan_save_stacktrace();

// Helper method to allow printing stack location in insane mode
void __nsan_print_stacktrace(uint32_t StackId);

size_t __nsan_get_shadowscale();

// When testing, we will not link to nsan runtime, so those functions will be
// undefined We declare dummy function to allow compilation
#ifdef DUMMY_NSAN_INTERFACE

void __nsan_dump_stacktrace() {}

uint32_t __nsan_save_stacktrace() { return 0; }

void __nsan_print_stacktrace(uint32_t StackId) {}

// FIXME: here we need to return a dummy value, but we don't know what's the
// shadow memory size, so this may lead to a backend crash
size_t __nsan_get_shadowscale() { return 2; }

#endif

namespace insane::utils {

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

size_t GetNSanShadowScale() {
  size_t res = __nsan_get_shadowscale();
  return res;
}

AsciiColor AsciiColor::Red = "\033[31m";
AsciiColor AsciiColor::Yellow = "\033[33m";
AsciiColor AsciiColor::Blue = "\033[34m";
AsciiColor AsciiColor::Cyan = "\033[36m";
AsciiColor AsciiColor::Green = "\033[32m";
AsciiColor AsciiColor::Lime = "\033[92m";
AsciiColor AsciiColor::Magenta = "\033[95m";
AsciiColor AsciiColor::Gray = "\033[90m";
AsciiColor AsciiColor::Reset = "\033[0m";

const char *AsciiColor::str() const {
  if (InsaneContext::getInstance().Flags().getUseColor())
    return Str;
  return "";
}

std::ostream &operator<<(std::ostream &Os, const AsciiColor &Color) {
  if (dynamic_cast<std::fstream *>(&Os))
    return Os;
  if (InsaneContext::getInstance().Flags().getUseColor())
    Os << Color.Str;
  return Os;
}

} // namespace insane::utils