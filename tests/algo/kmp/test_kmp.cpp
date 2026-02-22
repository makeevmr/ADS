#include <gtest/gtest.h>

#include "algo/expect_equality.hpp"
#include "algo/kmp/kmp.hpp"

using namespace ads::algo;
using namespace ads::algo::kmp;

// TODO: add tests
TEST(KMP, Test1) {
  expectVectorEquality(kmpSubstrSearch("ababcabcababc", "abc"), {2, 5, 10});
  expectVectorEquality(kmpSubstrSearch("aaaaa", "aa"), {0, 1, 2, 3});
  expectVectorEquality(kmpSubstrSearch("abcdef", "gh"), {});
  expectVectorEquality(kmpSubstrSearch("", "a"), {});
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
