/**
 * @file Interflop.cpp
 * @author Mathys JAM (mathys.jam@gmail.com)
 * @brief Module constructor and startup logic.
 * @version 0.5.0
 * @date 2021-07-20
 * 
 * 
 */

#include <iostream>
#include <iomanip>
#include "Context.hpp"

using namespace interflop;

// Warning : since we're in a module constructor, some objects may still be uninitialized, like std::cout
extern "C"
void __interflop_init()
{
  std::srand(time(NULL));
}
