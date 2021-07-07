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
#include "runtimes/DoublePrec.hpp"
#include "runtimes/MCASync.hpp"

using RuntimeInfo = mcasync::MSASyncRuntimeInfo;

// We need to know the used runtime at compile time so hot functions may be
// inlined
// We may also use #ifdef for every available runtime
template <typename FPType> using Runtime = RuntimeInfo::Runtime<FPType>;

using shadow128_t = RuntimeInfo::shadow128_t;
using shadow256_t = RuntimeInfo::shadow256_t;

#include "Shadow.hpp"

// Actually define the runtime
#define DOUBLEPREC_DEFINITION
#include "runtimes/DoublePrec.hpp"
#undef DOUBLEPREC_DEFINITION

// Actually define the runtime
#define MCASYNC_DEFINITION
#include "runtimes/MCASync.hpp"
#undef MCASYNC_DEFINITION