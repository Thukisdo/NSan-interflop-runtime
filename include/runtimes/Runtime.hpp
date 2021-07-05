#pragma once

#include "DoublePrec.hpp"

// We need to know the used runtime at compile time so hot functions can be
// inlined
// We may also use #ifdef for every available runtime
template <typename FPType> using Runtime = DoublePrecRuntime<FPType>;