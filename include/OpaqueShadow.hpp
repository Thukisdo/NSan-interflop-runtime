/**
 * @file Shadow.hpp
 * @author Mathys JAM (mathys.jam@ens.uvsq.fr)
 * @brief Defines opaque Shadow types and utilitary functions.
 * @version 1.1.0
 * @date 2021-08-30
 *
 *
 */

#pragma once
#include "Utils.hpp"
#include <iostream>

namespace insane {

struct OpaqueShadow {};

struct OpaqueLargeShadow {};
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

} // namespace insane