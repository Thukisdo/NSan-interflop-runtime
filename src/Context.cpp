/**
 * @file Context.cpp
 * @author Mathys JAM (mathys.jam@gmail.com)
 * @brief Context implementation
 * @version 0.5.0
 * @date 2021-07-20
 * 
 * 
 */

#include "Context.hpp"

namespace interflop {

// Global context object
InterflopContext Context;

InterflopContext::InterflopContext() {

  Stats = std::make_unique<RuntimeStats>();
  // Boilerplate code required to pre-allocate backend for every type
  // We could lazily create the backend, but it is probably  more efficient
  // as it avoids checking if backend is nullptr every time we need to use it
  Backends[utils::kFloat] = makeBackend<float>();
  Backends[utils::kV2Float] = makeBackend<v2float>();
  Backends[utils::kV4Float] = makeBackend<v4float>();
  Backends[utils::kV8Float] = makeBackend<v8float>();
  Backends[utils::kDouble] = makeBackend<double>();
  Backends[utils::kV2Double] = makeBackend<v2double>();
  Backends[utils::kV4Double] = makeBackend<v4double>();
  Backends[utils::kV8Double] = makeBackend<v8double>();
}

} // namespace interflop