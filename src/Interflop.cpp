#include <iostream>
#include <iomanip>
#include "Context.hpp"

using namespace interflop;
InterflopContext interflop::Context;

// Warning : since we're in a module constructor, some objects may still be uninitialized, like std::cout
extern "C"
void __interflop_init()
{
  std::srand(time(NULL));
}
