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

namespace insane::mcasync {

// We use 4x shadow memory, so our shadows should be either 128 or 256 bytes for
// large
struct MCASyncShadow {
  float val[2];

  // helper methods for printing / acessing one of the shadow float
  friend std::ostream &operator<<(std::ostream &os, MCASyncShadow const &s);

  // inline attribute is needed to prevent multiple definition on top of the
  // obvious performance reason
  inline float operator[](size_t const index) const {
    assert(index < 3);
    return val[index];
  }

  float mean() const { return (val[0] + val[1]) / 2; }
};

struct MCASyncLargeShadow {
  double val[2];

  // helper methods for printing / acessing one of the shadow double
  friend std::ostream &operator<<(std::ostream &os,
                                  MCASyncLargeShadow const &s);

  // inline attribute is needed to prevent multiple definition on top of the
  // obvious performance reason
  inline double operator[](size_t const index) const {
    assert(index < 3);
    return val[index];
  }

  float mean() const { return (val[0] + val[1]) / 2; }
};

static_assert(sizeof(MCASyncShadow) == 8, "Invalid shadow size");
static_assert(sizeof(MCASyncLargeShadow) == 16, "Invalid Large shadow size");

// Adapted from a Julia rounding code
// Voluntary making it avaible externally for testing purposes
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
float StochasticRound(double x);
double StochasticRound(__float128 x);
} // namespace insane::mcasync