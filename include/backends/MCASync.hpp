/**
 * @file MCASync.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr), Pablo Oliveira
 * (pablo.oliveira.uvsq.fr)
 * @brief MCA Synchrone backend, using 3 orbitals
 * @version 0.7.0
 * @date 2021-07-20
 *
 *
 */

#pragma once
#include "Backend.hpp"
#include "Flags.hpp"
#include "OpaqueShadow.hpp"
#include "Utils.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>

namespace interflop {

  // Adapted from a Julia rounding code
  // https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
  float StochasticRound(double x);
  double StochasticRound(__float128 x);
} // namespace interflop