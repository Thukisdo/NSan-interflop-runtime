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

struct OpaqueShadow {};

struct OpaqueLargeShadow {};

typedef OpaqueShadow *v2OpaqueShadow[2];
typedef OpaqueShadow *v4OpaqueShadow[4];
typedef OpaqueShadow *v8OpaqueShadow[8];
typedef OpaqueShadow *v16OpaqueShadow[16];

typedef OpaqueLargeShadow *v2OpaqueLargeShadow[2];
typedef OpaqueLargeShadow *v4OpaqueLargeShadow[4];
typedef OpaqueLargeShadow *v8OpaqueLargeShadow[8];

template <typename FPType> struct OpaqueShadowTypeFor {
  using Type = typename std::conditional_t<std::is_same_v<FPType, float>,
                                           OpaqueShadow, OpaqueLargeShadow>;
};

template <typename FPType>
using OpaqueShadowTypeFor_t = typename OpaqueShadowTypeFor<FPType>::Type;

template <typename Type, size_t Size> struct MetaFloat {
  using ScalarType = Type;
  using ShadowType = OpaqueShadowTypeFor_t<ScalarType>;
  static constexpr size_t VectorSize = Size;
  using FPType __attribute__((vector_size(sizeof(ScalarType) * VectorSize))) =
      ScalarType;
};

template <typename Type> struct MetaFloat<Type, 1> {
  using ScalarType = Type;
  using ShadowType = OpaqueShadowTypeFor_t<ScalarType>;
  static constexpr size_t VectorSize = 1;
  using FPType = ScalarType;
};

} // namespace interflop