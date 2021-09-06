/**
 * @file rounding.cpp
 * @author Mathys JAM (mathys.jam@gmail.com)
 * @brief Polynomial evaluation and rounding
 * @version 1.0
 * @date 2021-08-31
 * 
 * 
 */

#include "backends/MCASync.hpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

using namespace insane::mcasync;

template <typename FPType> struct ExtendedFPType {
  using type =
      std::conditional_t<std::is_same_v<float, FPType>, double, __float128>;
};

template <typename FPType>
using ExtendedFPType_t = typename ExtendedFPType<FPType>::type;

template <typename FPType> std::string FPTypeToStr() {
  if constexpr (std::is_same_v<float, FPType>)
    return "float";
  else if constexpr (std::is_same_v<double, FPType>)
    return "double";
  return "longdouble";
}

/* Expanded naïve implementation of the polynomial evaluation */
/* D. Stott Parker, MCA, section 8.1.2 pp.52-54               */
template <typename FPType> FPType ExpandedEvaluation(FPType x) {

  using ExtendedFP = ExtendedFPType_t<FPType>;

  /* Coefficients of the Tchebychev polynomial */
  constexpr FPType a[] = {1.,       -200.,     6600.,    -84480.,
                          549120.,  -2050048., 4659200., -6553600.,
                          5570560., -2621440., 524288.};

  FPType r = a[0];
  FPType x2 = StochasticRound((ExtendedFP)x * (ExtendedFP)x);
  FPType p = x2;

  for (int i = 1; i <= 10; i++) {
    r = StochasticRound((ExtendedFP)r +
                        StochasticRound((ExtendedFP)a[i] * (ExtendedFP)p));
    p = StochasticRound((ExtendedFP)p * (ExtendedFP)x2);
  }

  return r;
}

template <typename FPType> void Test() {
  
  static constexpr FPType kStep = 0.001;

  std::string filename = "out_" + FPTypeToStr<FPType>() + ".dat";
  std::ofstream output(filename);
  output << std::setprecision(16);

  for (FPType i = 0.5; i <= 1.0; i += kStep) {
    for (size_t j = 0; j < 20; j++) {
      FPType res = ExpandedEvaluation<FPType>(i);
      output << j << " " << i << " " << res << "\n";
    }
  }
}

int main() {
  std::array<std::thread, 2> threads;

  threads[0] = std::thread(Test<float>);
  threads[1] = std::thread(Test<double>);
  Test<long double>();

  std::for_each(threads.begin(), threads.end(),
                [](std::thread &t) { t.join(); });

  return 0;
}
