#pragma once

enum FCmpOpcode {
  OrderedFCmp,
  FCmp_oeq,
  FCmp_one,
  FCmp_ogt,
  FCmp_olt,
  // FIXME Should we handle Unordered comparisons ?
  UnorderedFCmp,
  FCmp_ueq,
  FCmp_une,
  FCmp_ugt,
  FCmp_ult
};

// Define the shadow types
#include "backends/DoublePrec.hpp"
#include "backends/MCASync.hpp"

// using RuntimeInfo = mcasync::MSASyncRuntimeInfo;
using RuntimeInfo = doubleprec::DoublePrecRuntimeInfo;

// We need to know the used runtime at compile time so hot functions may be
// inlined
// We may also use #ifdef for every available runtime
template <typename FPType> using Runtime = RuntimeInfo::Runtime<FPType>;

using shadow128_t = RuntimeInfo::shadow128_t;
using shadow256_t = RuntimeInfo::shadow256_t;

#include "Shadow.hpp"

// Actually define the runtime
#ifdef BACKEND_DEFINITION
#define RUNTIME_DEFINITION

#include "backends/DoublePrec.hpp"

#undef RUNTIME_DEFINITION
#endif
