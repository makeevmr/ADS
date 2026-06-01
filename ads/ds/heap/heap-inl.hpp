#ifndef ADS_DS_HEAP_HEAP_INL_HPP_
#error "Direct inclusion of this file is not allowed, include heap.hpp"
// For the sake of sane code completion.
#include "heap.hpp"
#endif

#include <stdexcept>

namespace NAds::NDs::NHeap {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Compare>
THeap<T, Compare>::THeap()
    : data_(nullptr),
      size_(0),
      capacity_(0),
      comparator_(Compare()) {}

template <typename T, typename Compare>
THeap<T, Compare>::THeap(const TSizeType capacity)
    : data_(reinterpret_cast<TValueType*>(
          ::operator new(sizeof(TValueType) * capacity))),
      size_(0),
      capacity_(capacity),
      comparator_(Compare()) {}

template <typename T, typename Compare>
THeap<T, Compare>::THeap(TValueType* construct_from_data, TSizeType size)
    : data_(reinterpret_cast<TValueType*>(
          ::operator new(sizeof(TValueType) * size))),
      size_(size),
      capacity_(size),
      comparator_(Compare()) {
  uninitializedCopy(data_, construct_from_data, size);
}

template <typename T, typename Compare>
THeap<T, Compare>::THeap(const THeap<T, Compare>& other)
    : data_(reinterpret_cast<TValueType*>(
          ::operator new(sizeof(TValueType) * other.capacity_))),
      size_(other.size_),
      capacity_(other.capacity_),
      comparator_(Compare()) {
  uninitializedCopy(data_, other);
}

template <typename T, typename Compare>
THeap<T, Compare>& THeap<T, Compare>::operator=(
    const THeap<T, Compare>& other) {
  if (this != &other) {
    TValueType* new_data = reinterpret_cast<TValueType*>(
        ::operator new(sizeof(TValueType) * other.capacity_));
    uninitializedCopy(new_data, other);
    free(data_, size_);
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
  }
  return *this;
}

template <typename T, typename Compare>
THeap<T, Compare>::THeap(THeap&& other) noexcept
    : data_(nullptr),
      size_(0),
      capacity_(0),
      comparator_(Compare()) {
  swap(other);
}

template <typename T, typename Compare>
THeap<T, Compare>& THeap<T, Compare>::operator=(
    THeap<T, Compare>&& other) noexcept {
  if (this != &other) {
    free(data_, size_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    swap(other);
  }
  return *this;
}

template <typename T, typename Compare>
THeap<T, Compare>::~THeap() {
  free(data_, size_);
}

// Element access
template <typename T, typename Compare>
[[nodiscard]] typename THeap<T, Compare>::TConstReference
THeap<T, Compare>::top() const {
  if (size_ == 0) {
    throw std::length_error("Heap is empty");
  }
  return data_[0];
}

// Capacity
template <typename T, typename Compare>
[[nodiscard]] bool THeap<T, Compare>::empty() const noexcept {
  return size_ == 0;
}

template <typename T, typename Compare>
[[nodiscard]] typename THeap<T, Compare>::TSizeType THeap<T, Compare>::getSize()
    const noexcept {
  return size_;
}

// Modifiers
template <typename T, typename Compare>
void THeap<T, Compare>::push(const TValueType& value) {
  if (size_ == capacity_) {
    this->resize();
  }
  new (data_ + size_) TValueType(value);
  ++size_;
  siftingUp(size_ - 1);
}

template <typename T, typename Compare>
void THeap<T, Compare>::push(const TValueType&& value) {
  if (size_ == capacity_) {
    this->resize();
  }
  new (data_ + size_) TValueType(std::move(value));
  ++size_;
  siftingUp(size_ - 1);
}

template <typename T, typename Compare>
void THeap<T, Compare>::pop() {
  if (size_ == 0) {
    throw std::length_error("Heap is empty");
  }
  data_[0] = std::move(data_[size_ - 1]);
  if (!std::is_trivially_destructible_v<T>) {
    (data_ + size_ - 1)->~TValueType();
  }
  --size_;
  siftingDown(0);
}

template <typename T, typename Compare>
void THeap<T, Compare>::swap(THeap<T, Compare>& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T, typename Compare>
void THeap<T, Compare>::free(TValueType* data_to_free,
                             TSizeType destructor_calls) noexcept {
  if (!std::is_trivially_destructible_v<TValueType>) {
    TSizeType destroyed_objects = 0;
    while (destroyed_objects < destructor_calls) {
      (data_to_free + destroyed_objects)->~TValueType();
      ++destroyed_objects;
    }
  }
  ::operator delete(data_to_free);
}

template <typename T, typename Compare>
void THeap<T, Compare>::uninitializedCopy(TValueType* copy_to,
                                          const THeap<T, Compare>& copy_from) {
  TSizeType copied_objects = 0;
  try {
    for (; copied_objects < copy_from.size_; ++copied_objects) {
      new (copy_to + copied_objects)
          TValueType(copy_from.data_[copied_objects]);
    }
  } catch (...) {
    free(copy_to, copied_objects);
    throw;
  }
}

template <typename T, typename Compare>
void THeap<T, Compare>::uninitializedCopy(TValueType* copy_to,
                                          const TValueType* copy_from,
                                          TSizeType size) {
  TSizeType copied_objects = 0;
  try {
    for (; copied_objects < size; ++copied_objects) {
      new (copy_to + copied_objects) TValueType(copy_from[copied_objects]);
    }
  } catch (...) {
    free(copy_to, copied_objects);
    throw;
  }
}

template <typename T, typename Compare>
[[nodiscard]] typename THeap<T, Compare>::TSizeType THeap<T, Compare>::getLeft(
    TSizeType index) const noexcept {
  return 2 * index + 1;
}

template <typename T, typename Compare>
[[nodiscard]] typename THeap<T, Compare>::TSizeType THeap<T, Compare>::getRight(
    TSizeType index) const noexcept {
  return 2 * index + 2;
}

template <typename T, typename Compare>
[[nodiscard]] typename THeap<T, Compare>::TSizeType
THeap<T, Compare>::getParent(TSizeType index) const noexcept {
  return (index - 1) / 2;
}

template <typename T, typename Compare>
void THeap<T, Compare>::resize() {
  TSizeType new_capacity = capacity_ > 0 ? capacity_ * 2 : capacity_ + 1;
  TValueType* new_data = reinterpret_cast<TValueType*>(
      ::operator new(sizeof(TValueType) * new_capacity));
  if (std::is_move_constructible_v<TValueType>) {
    TSizeType moved_objects = 0;
    while (moved_objects < size_) {
      new (new_data + moved_objects)
          TValueType(std::move(data_[moved_objects]));
      ++moved_objects;
    }
    ::operator delete(data_);
  } else {
    uninitializedCopy(new_data, *this);
    free(data_, size_);
  }
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T, typename Compare>
void THeap<T, Compare>::siftingDown(TSizeType index) noexcept(
    std::is_nothrow_swappable_v<TValueType>) {
  TSizeType iter_start_index = index;
  TSizeType iter_end_index = index;
  do {
    iter_start_index = iter_end_index;
    TSizeType left = getLeft(iter_start_index);
    TSizeType right = getRight(iter_start_index);
    if ((left < size_) && (comparator_(data_[left], data_[iter_end_index]))) {
      iter_end_index = left;
    }
    if ((right < size_) && (comparator_(data_[right], data_[iter_end_index]))) {
      iter_end_index = right;
    }
    if (iter_start_index != iter_end_index) {
      std::swap(data_[iter_start_index], data_[iter_end_index]);
    }
  } while (iter_start_index != iter_end_index);
}

template <typename T, typename Compare>
void THeap<T, Compare>::siftingUp(TSizeType index) noexcept(
    std::is_nothrow_swappable_v<TValueType>) {
  bool is_sifting_complete = false;
  while ((index > 0) && !is_sifting_complete) {
    is_sifting_complete = true;
    TSizeType parent = getParent(index);
    if (comparator_(data_[index], data_[parent])) {
      std::swap(data_[index], data_[parent]);
      index = parent;
      is_sifting_complete = false;
    }
  }
}

template <typename T, typename Compare>
void THeap<T, Compare>::makeHeap() noexcept(std::is_nothrow_swappable_v<T>) {
  for (TSizeType i = (size_ / 2 - 1); i > 0; --i) {
    siftingDown(i);
  }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NHeap
