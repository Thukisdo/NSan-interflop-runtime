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

namespace interflop::mcasync {

struct MCASyncShadow128 {
  float val[3];
  uint32_t padding[1];

  // helper methods for printing / acessing one of the shadow float
  friend std::ostream &operator<<(std::ostream &os, MCASyncShadow128 const &s);

  // inline attribute is needed to prevent multiple definition on top of the
  // obvious performance reason
  inline float operator[](size_t const index) const {
    assert(index < 3);
    return val[index];
  }
};
struct MCASyncShadow256 {
  double val[3];
  uint64_t padding[1];

  // helper methods for printing / acessing one of the shadow double
  friend std::ostream &operator<<(std::ostream &os, MCASyncShadow256 const &s);

  // inline attribute is needed to prevent multiple definition on top of the
  // obvious performance reason
  inline double operator[](size_t const index) const {
    assert(index < 3);
    return val[index];
  }
};

// Adapted from a Julia rounding code
// Voluntary making it avaible externally for testing purposes
// https://github.com/milankl/StochasticRounding.jl/blob/main/src/float32sr.jl
float StochasticRound(double x);
double StochasticRound(__float128 x);
} // namespace interflop::mcasync