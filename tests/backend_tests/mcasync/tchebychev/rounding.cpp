#include "backends/MCASync.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

using namespace interflop::mcasync;

#ifdef FLOAT
using Real = float;
using ExtendedReal = double;
#elif defined(DOUBLE)
using Real = double;
using ExtendedReal = __float128;
#endif

/* Coefficients of the Tchebychev polynomial */
constexpr Real a[] = {1.,       -200.,     6600.,    -84480.,
                       549120.,  -2050048., 4659200., -6553600.,
                       5570560., -2621440., 524288.};

/* Expanded naïve implementation of the polynomial evaluation */
/* D. Stott Parker, MCA, section 8.1.2 pp.52-54               */
Real expanded(Real x) {
  Real r = a[0];
  Real x2 = StochasticRound((ExtendedReal)x * x);
  Real p = x2;

  for (int i = 1; i <= 10; i++) {
    r = StochasticRound((ExtendedReal)r + StochasticRound((ExtendedReal)a[i] * p));
    p = StochasticRound((ExtendedReal)p * x2);
  }

  return r;
}

int main(int argc, char **argv) {
  srand(time(nullptr));
  static constexpr Real kStep = 0.001;
  std::ofstream output("out.dat");
  output << std::setprecision(16);

  for (Real i = 0.5; i <= 1.0; i += kStep) {
    for (size_t j = 0; j < 20; j++) {
      Real res = expanded(i);
      output << j << " " << i << " " << res << "\n";
    }
  }
  return 0;
}
