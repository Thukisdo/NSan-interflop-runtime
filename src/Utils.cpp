/**
 * @file Utils.cpp
 * @author Mathys JAM (mathys.jam@gmail.com)
 * @brief Utilitary functions implementation
 * @version 0.5.0
 * @date 2021-07-20
 * 
 */
#include "Utils.hpp"

namespace interflop::utils {

[[noreturn]] void unreachable(const char *str) noexcept {
  if (str)
    std::cout << str << std::endl;
  std::cout << "Interflop unreachable called !\n";
  DumpStacktrace();
  exit(1);
}

void DumpStacktrace() noexcept {
  std::cout << "Interflop stacktrace : \n";
  __nsan_dump_stacktrace();
}

}