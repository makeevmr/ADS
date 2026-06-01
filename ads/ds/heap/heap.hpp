#pragma once

#include <type_traits>

namespace NAds::NDs::NHeap {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Compare>
class THeap {
public:
  using TValueType = T;
  using TSizeType = std::size_t;
  using TValueCompare = Compare;
  using TReference = T&;
  using TConstReference = const T&;

  THeap();

  explicit THeap(TSizeType capacity);

  THeap(TValueType* construct_from_data, TSizeType size);

  THeap(const THeap<T, Compare>& other);

  THeap<T, Compare>& operator=(const THeap<T, Compare>& other);

  THeap(THeap&& other) noexcept;

  THeap<T, Compare>& operator=(THeap<T, Compare>&& other) noexcept;

  ~THeap();

  // Element access
  [[nodiscard]] TConstReference top() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] TSizeType getSize() const noexcept;

  // Modifiers
  void push(const TValueType& value);

  void push(const TValueType&& value);

  void pop();

private:
  void swap(THeap<T, Compare>& other) noexcept;

  static void free(TValueType* data_to_free,
                   TSizeType destructor_calls) noexcept;

  static void uninitializedCopy(TValueType* copy_to,
                                const THeap<T, Compare>& copy_from);

  static void uninitializedCopy(TValueType* copy_to,
                                const TValueType* copy_from, TSizeType size);

  [[nodiscard]] TSizeType getLeft(TSizeType index) const noexcept;

  [[nodiscard]] TSizeType getRight(TSizeType index) const noexcept;

  [[nodiscard]] TSizeType getParent(TSizeType index) const noexcept;

  void resize();

  void siftingDown(TSizeType index) noexcept(std::is_nothrow_swappable_v<T>);

  void siftingUp(TSizeType index) noexcept(std::is_nothrow_swappable_v<T>);

  void makeHeap() noexcept(std::is_nothrow_swappable_v<T>);

  TValueType* data_;
  TSizeType size_;
  TSizeType capacity_;
  TValueCompare comparator_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NHeap

#define ADS_DS_HEAP_HEAP_INL_HPP_
#include "heap-inl.hpp"
#undef ADS_DS_HEAP_HEAP_INL_HPP_
