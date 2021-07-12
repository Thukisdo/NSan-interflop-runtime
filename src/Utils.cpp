#include "Utils.hpp"

namespace utils {

[[noreturn]] void unreachable(const char *str) {
  if (str)
    std::cout << str << std::endl;
  std::cout << "Interflop unreachable called !\n";
  DumpStacktrace();
  exit(1);
}

} // namespace utils