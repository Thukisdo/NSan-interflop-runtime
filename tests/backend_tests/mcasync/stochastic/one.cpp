#include "backends/MCASync.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace interflop::mcasync;

int main(int argc, char *argv[]) {
  srand(time(nullptr));
  std::ofstream FloatOutput("out_float.dat");
  std::ofstream DoubleOutput("out_double.dat");

  FloatOutput << "x\n" << std::setprecision(32);
  DoubleOutput << "x\n" << std::setprecision(64);
  for (int I = 0; I < 1000; ++I) {
    double dx = StochasticRound((__float128)1);
    float fx =  StochasticRound((double)0.2345);
    FloatOutput << fx << std::endl;
    DoubleOutput << dx << std::endl;
  }
}