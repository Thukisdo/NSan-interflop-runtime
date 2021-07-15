#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

namespace interflop {
namespace mcasync {

float StochasticRound(float x);
double StochasticRound(double x);

} // namespace mcasync
} // namespace interflop

using namespace interflop::mcasync;

/* Coefficients of the Tchebychev polynomial */
constexpr float a[] = {1.,       -200.,     6600.,    -84480.,
                       549120.,  -2050048., 4659200., -6553600.,
                       5570560., -2621440., 524288.};

/* Expanded naïve implementation of the polynomial evaluation */
/* D. Stott Parker, MCA, section 8.1.2 pp.52-54               */
float expanded(float x) {
  float r = a[0];
  float x2 = StochasticRound((double)x * (double)x);
  float p = x2;

  for (int i = 1; i <= 10; i++) {
    // Wrong results arise from the following casts
    r = StochasticRound((double)r + StochasticRound((double)a[i] * (double)p));
    p = StochasticRound((double)p * (double)x2);
  }

  return r;
}

int main(int argc, char **argv) {
  srand(time(nullptr));
  static constexpr float kStep = 0.001;
  std::ofstream output("out.dat");
  output << std::setprecision(16);

  for (float i = 0.5; i <= 1.0; i += kStep) {
    for (size_t j = 0; j < 20; j++) {
      float res = expanded(i);
      output << j << " " << i << " " << res << "\n";
    }
  }
  return 0;
}
