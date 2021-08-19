/**
 * @file Shadow.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Defines the opaque Shadow type and utilitary functions.
 * @version 1.0.0
 * @date 2021-07-20
 * 
 * 
 */

#pragma once
#include "Utils.hpp"
#include <iostream>

namespace interflop {
struct OpaqueShadow {
  
};

struct OpaqueLargeShadow {
  
};

typedef OpaqueShadow *v2OpaqueShadow[2];
typedef OpaqueShadow *v4OpaqueShadow[4];
typedef OpaqueShadow *v8OpaqueShadow[8];
typedef OpaqueShadow *v16OpaqueShadow[16];

typedef OpaqueLargeShadow *v2OpaqueLargeShadow[2];
typedef OpaqueLargeShadow *v4OpaqueLargeShadow[4];
typedef OpaqueLargeShadow *v8OpaqueLargeShadow[8];

// Boilerplate code to ease backend implementation.
template <typename> struct FPTypeInfo {};

template <> struct FPTypeInfo<float> {
  using ShadowType = OpaqueShadow;
  using ScalarType = float;
  static constexpr utils::FloatType Type = utils::kFloat;
  static constexpr size_t VectorSize = 1;
};

template <> struct FPTypeInfo<v2float> {
  using ShadowType = OpaqueShadow;
  using ScalarType = float;
  static constexpr utils::FloatType Type = utils::kV2Float;
  static constexpr size_t VectorSize = 2;
};

template <> struct FPTypeInfo<v4float> {
  using ShadowType = OpaqueShadow;
  using ScalarType = float;
  static constexpr utils::FloatType Type = utils::kV4Float;
  static constexpr size_t VectorSize = 4;
};

template <> struct FPTypeInfo<v8float> {
  using ShadowType = OpaqueShadow;
  using ScalarType = float;
  static constexpr utils::FloatType Type = utils::kV4Float;
  static constexpr size_t VectorSize = 8;
};

// FIXME: We should probably only enable this only if 512 bits registers are available
template <> struct FPTypeInfo<v16float> {
  using ShadowType = OpaqueShadow;
  using ScalarType = float;
  static constexpr utils::FloatType Type = utils::kV4Float;
  static constexpr size_t VectorSize = 16;
};

template <> struct FPTypeInfo<double> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = double;
  static constexpr utils::FloatType Type = utils::kDouble;
  static constexpr size_t VectorSize = 1;
};

template <> struct FPTypeInfo<v2double> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = double;
  static constexpr utils::FloatType Type = utils::kV2Double;
  static constexpr size_t VectorSize = 2;
};

template <> struct FPTypeInfo<v4double> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = double;
  static constexpr utils::FloatType Type = utils::kV4Double;
  static constexpr size_t VectorSize = 4;
};

// FIXME: We should probably only enable this only if 512 bits registers are available
template <> struct FPTypeInfo<v8double> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = double;
  static constexpr utils::FloatType Type = utils::kV8Double;
  static constexpr size_t VectorSize = 8;
};

template <> struct FPTypeInfo<long double> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = long double;
  static constexpr utils::FloatType Type = utils::klDouble;
  static constexpr size_t VectorSize = 1;
};

template <> struct FPTypeInfo<v2ldouble> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = long double;
  static constexpr utils::FloatType Type = utils::kV2lDouble;
  static constexpr size_t VectorSize = 2;
};

template <> struct FPTypeInfo<v4ldouble> {
  using ShadowType = OpaqueLargeShadow;
  using ScalarType = long double;
  static constexpr utils::FloatType Type = utils::kV4lDouble;
  static constexpr size_t VectorSize = 4;
};


} // namespace interflop