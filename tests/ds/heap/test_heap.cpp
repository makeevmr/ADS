#include <gtest/gtest.h>

#include "ds/heap/heap.hpp"
#include "ds/heap/compare.hpp"

using namespace NAds::NDs::NHeap;

TEST(Heap, SimpleTest) {
  const int heap_size = 1000;

  THeap<int, TMoreCompare<int>> min_heap;
  for (int i = heap_size; i > 0; --i) {
    min_heap.push(i);
  }
  EXPECT_EQ(min_heap.size(), heap_size);
  for (int i = heap_size; i > 0; --i) {
    EXPECT_EQ(min_heap.top(), i);
    min_heap.pop();
  }
  EXPECT_TRUE(min_heap.empty());

  THeap<int, TLessCompare<int>> max_heap;
  for (int i = heap_size; i > 0; --i) {
    max_heap.push(i);
  }
  EXPECT_EQ(max_heap.size(), heap_size);
  for (int i = 1; i <= heap_size; ++i) {
    EXPECT_EQ(max_heap.top(), i);
    max_heap.pop();
  }
  EXPECT_TRUE(max_heap.empty());
}

TEST(Heap, Copy) {
  const int heap_size = 12345;

  THeap<int, TMoreCompare<int>> min_heap;
  for (int i = heap_size; i > 0; --i) {
    min_heap.push(i);
  }

  THeap<int, TMoreCompare<int>> min_heap_copy(min_heap);
  EXPECT_EQ(min_heap.size(), min_heap_copy.size());

  for (int i = heap_size; i > 0; --i) {
    EXPECT_EQ(min_heap.top(), min_heap_copy.top());
    EXPECT_EQ(min_heap_copy.top(), i);
    min_heap.pop();
    min_heap_copy.pop();
  }
  EXPECT_TRUE(min_heap.empty());
  EXPECT_TRUE(min_heap_copy.empty());
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
