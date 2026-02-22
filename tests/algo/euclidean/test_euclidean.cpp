#include <gtest/gtest.h>

#include "algo/euclidean/euclidean.hpp"

using namespace ads::algo::euclidean;

// TODO: add tests
TEST(Euclidean, TestGCD) {
  EXPECT_EQ(gcd(30, 24), 6);
  EXPECT_EQ(gcd(30, 0), 30);
  EXPECT_EQ(gcd(30, 100), 10);
}

TEST(Euclidean, TestLCM) {
  EXPECT_EQ(lcm(3, 24), 24);
  EXPECT_EQ(lcm(30, 4), 60);
  EXPECT_EQ(lcm(30, 15), 30);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
