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

namespace insane::doubleprec {

// We use 2x shadow memory, so our shadows should be either 64 or 128 bytes for large
struct DoublePrecShadow {
  double val;
};

struct DoublePrecLargeShadow {
  __float128 val;
};

static_assert(sizeof(DoublePrecShadow) == 8, "invalid DoublePrec shadow size");
static_assert(sizeof(DoublePrecLargeShadow) == 16, "invalid DoublePrec Large shadow size");

} // namespace insane::doubleprec