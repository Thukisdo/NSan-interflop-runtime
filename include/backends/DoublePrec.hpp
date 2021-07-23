/**
 * @file DoublePrec.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Double precision backend to mimic native NSan behaviour
 * @version 0.9.0
 * @date 2021-07-20
 *
 */

#pragma once
#include "Backend.hpp"
#include "Flags.hpp"
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

namespace interflop::doubleprec {

struct DoublePrecShadow128 {
  double val;
  // uint64_t padding[1];
};

struct DoublePrecShadow256 {
  __float128 val;
  // uint64_t padding[2];
};

} // namespace interflop::doubleprec