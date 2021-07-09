#pragma once
#include <iostream>
#include <memory>
#include "Backend.hpp"

namespace interflop {

class RuntimeStats {
public:
private:
};

class Context {
public:

  std::string const &getBackendName() const;

  RuntimeStats* getStats();
  RuntimeStats const* getStats() const;

private:

  std::unique_ptr<RuntimeStats> Stats;
};

} // namespace interflop
