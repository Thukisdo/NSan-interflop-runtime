#include "backends/MCASync.hpp"
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

using namespace interflop::mcasync;

/* Coefficients of the Tchebychev polynomial */
constexpr float a[] = {1.,       -200.,     6600.,    -84480.,
                       549120.,  -2050048., 4659200., -6553600.,
                       5570560., -2621440., 524288.};

/* Expanded naïve implementation of the polynomial evaluation */
/* D. Stott Parker, MCA, section 8.1.2 pp.52-54               */
float expanded(float x) {
  float r = a[0];
  float x2 = StochasticRound((double)x * x);
  float p = x2;

  for (int i = 1; i <= 10; i++) {
    r = StochasticRound((double)r + StochasticRound((double)a[i] * p));
    p = StochasticRound((double)p * x2);
  }

  return r;
}

// FIXME : doesn't work with 128 bits computation
/* double expanded(double x) {
  double r = a[0];
  double x2 = StochasticRound((__float128)x * x);
  double p = x2;

  for (int i = 1; i <= 10; i++) {
    // Wrong results arise from the following casts
    r = StochasticRound((__float128)r + StochasticRound((__float128)a[i] * p));
    p = StochasticRound((__float128)p * x2);
  }

  return r;
} */

int main(int argc, char **argv) {
  srand(time(nullptr));
  static constexpr float kStep = 0.001;
  std::ofstream output("out.dat");
  output << std::setprecision(16);

  for (double i = 0.5; i <= 1.0; i += kStep) {
    for (size_t j = 0; j < 20; j++) {
      double res = expanded(i);
      output << j << " " << i << " " << res << "\n";
    }
  }
  return 0;
}
