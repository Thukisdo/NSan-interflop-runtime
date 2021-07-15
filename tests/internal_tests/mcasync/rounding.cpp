#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

namespace interflop {
namespace mcasync {

double StochasticRound(double x);
double StochasticRound(double x);

} // namespace mcasync
} // namespace interflop

using namespace interflop::mcasync;

/* Coefficients of the Tchebychev polynomial */
constexpr double a[] = {1.,       -200.,     6600.,    -84480.,
                       549120.,  -2050048., 4659200., -6553600.,
                       5570560., -2621440., 524288.};

/* Expanded naïve implementation of the polynomial evaluation */
/* D. Stott Parker, MCA, section 8.1.2 pp.52-54               */
double expanded(double x) {
  double r = a[0];
  double x2 = StochasticRound(x * x);
  double p = x2;

  for (int i = 1; i <= 10; i++) {
    r = StochasticRound(r + StochasticRound(a[i] * p));
    p = StochasticRound(p * x2);
  }

  return r;
}

int main(int argc, char **argv) {
  srand(time(nullptr));
  static constexpr double kStep = 0.001f;
  std::ofstream output("out.dat");
  output << std::setprecision(16);

  for (double i = 0.0f; i < 1.0f; i += kStep) {
    for (size_t j = 0; j < 20; j++) {
      double res = expanded(i);
      output << j << " " << i << " " << res << "\n";
    }
  }
  return 0;
}
