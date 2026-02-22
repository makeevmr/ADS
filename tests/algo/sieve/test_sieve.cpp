#include <gtest/gtest.h>

#include "algo/expect_equality.hpp"
#include "algo/sieve/sieve.hpp"

using namespace ads::algo;
using namespace ads::algo::sieve;

TEST(SieveOfEratosthenes, Test1) {
  std::vector<bool> expected_result = {false, false, true, true, false,
                                       true,  false, true, false};
  expectVectorEquality(createEratoSieve(8), expected_result);
}

TEST(SieveOfEratosthenes, Test2) {
  std::vector<bool> expected_result = {false, false, true};
  expectVectorEquality(createEratoSieve(2), expected_result);
}

TEST(SieveOfEratosthenes, Test3) {
  std::vector<bool> expected_result = {false, false, true,  true,  false, true,
                                       false, true,  false, false, false, true,
                                       false, true,  false, false, false};
  expectVectorEquality(createEratoSieve(16), expected_result);
}

TEST(SieveOfEratosthenes, ExpectThrow) {
  EXPECT_THROW(static_cast<void>(createEratoSieve(0)), std::runtime_error);
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
