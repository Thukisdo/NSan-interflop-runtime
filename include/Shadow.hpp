#pragma once
#include "Utils.hpp"
#include <iostream>

namespace interflop {
struct OpaqueShadow128 {
  uint64_t opaque[2];
};

struct OpaqueShadow256 {
  uint64_t opaque[4];
};

typedef OpaqueShadow128 *v2OpaqueShadow128[2];
typedef OpaqueShadow128 *v4OpaqueShadow128[4];
typedef OpaqueShadow128 *v8OpaqueShadow128[8];

typedef OpaqueShadow256 *v2OpaqueShadow256[2];
typedef OpaqueShadow256 *v4OpaqueShadow256[4];
typedef OpaqueShadow256 *v8OpaqueShadow256[8];

template <typename> struct FPTypeInfo {};

template <> struct FPTypeInfo<float> {
  using ShadowType = OpaqueShadow128;
  using ScalarType = float;
  static constexpr utils::FPType Type = utils::kFloat;
  static constexpr size_t VectorSize = 1;
};

template <> struct FPTypeInfo<v2float> {
  using ShadowType = OpaqueShadow128;
  using ScalarType = float;
  static constexpr utils::FPType Type = utils::kV2Float;
  static constexpr size_t VectorSize = 2;
};

template <> struct FPTypeInfo<v4float> {
  using ShadowType = OpaqueShadow128;
  using ScalarType = float;
  static constexpr utils::FPType Type = utils::kV4Float;
  static constexpr size_t VectorSize = 4;
};

template <> struct FPTypeInfo<double> {
  using ShadowType = OpaqueShadow256;
  using ScalarType = double;
  static constexpr utils::FPType Type = utils::kDouble;
  static constexpr size_t VectorSize = 1;
};

template <> struct FPTypeInfo<v2double> {
  using ShadowType = OpaqueShadow256;
  using ScalarType = double;
  static constexpr utils::FPType Type = utils::kV2Double;
  static constexpr size_t VectorSize = 2;
};

template <> struct FPTypeInfo<v4double> {
  using ShadowType = OpaqueShadow256;
  using ScalarType = double;
  static constexpr utils::FPType Type = utils::kV4Double;
  static constexpr size_t VectorSize = 4;
};

template <> struct FPTypeInfo<v8double> {
  using ShadowType = OpaqueShadow256;
  using ScalarType = double;
  static constexpr utils::FPType Type = utils::kV8Double;
  static constexpr size_t VectorSize = 8;
};
}