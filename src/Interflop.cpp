/**
 * @file Interflop.cpp
 * @author Mathys JAM (mmathys.jam@ens.uvsq.fr)
 * @brief Module constructor and startup logic.
 * @version 0.5.0
 * @date 2021-07-20
 * 
 * 
 */

#include <iostream>
#include <iomanip>
#include "Backend.hpp"
#include "Context.hpp"

using namespace interflop;

// Warning : since we're in a module constructor, some objects may still be uninitialized, like std::cout
extern "C"
void __interflop_init()
{
  backend_init();
}
