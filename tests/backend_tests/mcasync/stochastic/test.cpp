#include "backends/MCASync.hpp"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>

constexpr size_t N_SAMPLE = 1000;

namespace fs = std::filesystem;

using namespace interflop::mcasync;

void test_random() {
  std::ofstream FloatOutput("output/out_random.dat");

  FloatOutput << "x\n" << std::setprecision(16);
  for (int I = 0; I < N_SAMPLE; ++I) {
    FloatOutput <<  StochasticRound((double)1.89) << std::endl;
  }
}

void test_one() {
  std::ofstream FloatOutput("output/out_one.dat");

  FloatOutput << "x\n" << std::setprecision(16);
  for (int I = 0; I < N_SAMPLE; ++I) {
    float fx = StochasticRound((double)1);
    FloatOutput << fx << std::endl;
  }
}

int main(int argc, char *argv[]) {
  srand(time(nullptr));
  fs::create_directory("output");
  test_one();
  test_random();
}