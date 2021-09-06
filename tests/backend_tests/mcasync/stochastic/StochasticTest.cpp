#include "backends/MCASync.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>


using namespace insane::mcasync;

constexpr size_t N_SAMPLE = 100000;

// Helper methods to generate samples from a given input
void Sample(double Input, std::pair<size_t, size_t> &Counts) {

  // We know the bounds of the output value
  float roundup, rounddown, f32_round = Input;
  // Check wether the output is in range {x, x + ulp} or {x - ulp, x}
  if (f32_round <= Input) { 
    rounddown = f32_round;
    roundup = std::nextafter(f32_round, std::numeric_limits<float>::max());
  } else {
    roundup = f32_round;
    rounddown = std::nextafter(f32_round, std::numeric_limits<float>::min());
  }

  Counts = {0, 0};
  for (int I = 0; I < N_SAMPLE; ++I) {
    float x = StochasticRound(Input);
    if (x == rounddown)
      Counts.first++;
    else if (x == roundup)
      Counts.second++;
    else
      FAIL();
  }
}

// Helper method to compute the expected output probability of upward rounding
double ChanceRoundup(double x) {

  double xround = (float)x;
  if (xround == x)
    return 0.0;

  double xround_down, xround_up;

  if (xround < x) {
    xround_down = xround;
    xround_up =
        std::nextafter((float)xround, std::numeric_limits<float>::max());
  } else {
    xround_up = xround;
    xround_down =
        std::nextafter((float)xround, std::numeric_limits<float>::min());
  }
  return (x - xround_down) / (xround_up - xround_down);
}

void CalcSampleRatio(double X) {
  std::pair<size_t, size_t> Counts = {0, 0};
  Sample(X, Counts);

  // We check the rounding ratio is coherent with the expected outputs
  // probabilites
  const double p = ChanceRoundup(X);
  EXPECT_EQ(Counts.first + Counts.second, N_SAMPLE);
  // Chance of down rounding (1 - p)
  EXPECT_NEAR((double)Counts.first / N_SAMPLE, 1.0 - p, 2e-2);
  // Chanced of up rounding (p)
  EXPECT_NEAR((double)Counts.second / N_SAMPLE, p, 2e-2);
}

TEST(MCASync, RoundInfinite) {
  constexpr double Infinite = std::numeric_limits<double>::infinity();
  float X = StochasticRound(Infinite);
  // FIXME: we should expect Infinite
  EXPECT_TRUE(std::isinf(X));
}

TEST(MCASync, RoundNan) {
  constexpr double NaN = std::numeric_limits<double>::quiet_NaN();
  float X = StochasticRound(NaN);
  EXPECT_TRUE(std::isnan(X));
}

TEST(MCASync, RoundRandomFloat) {

  std::array<double, 8> Randoms = {2.13,  4.48,  68.15,   404.33,
                                   580.9, 605.7, 1200.36, 1234.5};

  for (double X : Randoms)
    CalcSampleRatio(X);
}

TEST(MCASync, RoundPowerOfTwo) {
  std::array<double, 8> Powers = {2, 4, 8, 16, 32, 64, 12, 256};

  // We expect one output for powers of two
  // So this test is kindof excessive
  for (double X : Powers)
    CalcSampleRatio(X);
  for (double X : Powers)
    CalcSampleRatio(-X);
  for (double X : Powers)
    CalcSampleRatio(1 / X);
  for (double X : Powers)
    CalcSampleRatio(-(1 / X));
}

#if FLT_HAS_SUBNORM

TEST(MCASync, RoundExactSubnormal) {
  std::array<float, 4> ExactValueTests = {0x1.1p+34f, 0x1.04p-20f, 0x1p+0f,
                                          0x1p-10f};

  for (float x : ExactValueTests) {
    float res = StochasticRound(x);
    EXPECT_FLOAT_EQ(res, x);
  }
}

TEST(MCASync, RoundInexactSubnormal) {
  std::array<float, 3> InexactValueTests = {
      0x1.999999999999ap-4, 0x1.ed33333333333p+6, 0x1.535afdf51cc65p-30};
  // @pablo.oliveira hand computed values
  std::array<float, 3> InexactValueResults = {0x1.99999ap-4, 0x1.ed3334p+6,
                                              0x1.535afep-30};

  // Coherency check between the check's expected values and the hand computed
  // ones
  for (float x : InexactValueResults)
    EXPECT_FLOAT_EQ(x, std::nextafter(x, std::numeric_limits<float>::max()));

  // CalcSampleRatio will use std::nextafter, so no need to check again against
  // the hand computed values if the last test succedeed
  for (int I = 0; I < InexactValueResults.size(); I++)
    CalcSampleRatio(InexactValueTests[I]);
}

#endif