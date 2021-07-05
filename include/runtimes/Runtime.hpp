#pragma once

// Define the shadow types
#include "DoublePrec.hpp"

// We need to know the used runtime at compile time so hot functions can be
// inlined
// We may also use #ifdef for every available runtime
template <typename FPType>
using Runtime = doubleprec::DoublePrecRuntime<FPType>;

// FIXME replace void with a given type ?
using shadow128_t = Runtime<void>::shadow128_t;
using shadow256_t = Runtime<void>::shadow256_t;

#include "Shadow.hpp"

enum FCmpOpcode {
  kOrderedFCmp,
  kFCmp_oeq,
  kFCmp_one,
  kFCmp_ogt,
  kFCmp_olt,
  kUnorderedFCmp,
  kFCmp_ueq,
  kFCmp_une,
  kFCmp_ugt,
  kFCmp_ult
};


// Actually define the runtime
#define DOUBLEPREC_DEFINITION
#include "DoublePrec.hpp"

template<typename FPType, FCmpOpcode Opcode>
using ShadowPredicate = doubleprec::ShadowPredicate<FPType, Opcode>;

template<typename FPType>
using ShadowFCmp = doubleprec::ShadowFCmp<FPType>;
