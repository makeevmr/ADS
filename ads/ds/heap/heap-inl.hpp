#ifndef ADS_DS_HEAP_HEAP_INL_HPP_
#error "Direct inclusion of this file is not allowed, include heap.hpp"
// For the sake of sane code completion.
#include "heap.hpp"
#endif

#include <stdexcept>

namespace NAds::NDs::NHeap {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TCompare>
THeap<T, TCompare>::THeap()
    : Data_(nullptr),
      Size_(0),
      Capacity_(0),
      Comparator_(TCompare()) {}

template <typename T, typename TCompare>
THeap<T, TCompare>::THeap(const TSizeType capacity)
    : Data_(reinterpret_cast<TValueType*>(
          ::operator new(sizeof(TValueType) * capacity))),
      Size_(0),
      Capacity_(capacity),
      Comparator_(TCompare()) {}

template <typename T, typename TCompare>
THeap<T, TCompare>::THeap(TValueType* construct_from_data, TSizeType size)
    : Data_(reinterpret_cast<TValueType*>(
          ::operator new(sizeof(TValueType) * size))),
      Size_(size),
      Capacity_(size),
      Comparator_(TCompare()) {
  uninitializedCopy(Data_, construct_from_data, size);
}

template <typename T, typename TCompare>
THeap<T, TCompare>::THeap(const THeap<T, TCompare>& other)
    : Data_(reinterpret_cast<TValueType*>(
          ::operator new(sizeof(TValueType) * other.Capacity_))),
      Size_(other.Size_),
      Capacity_(other.Capacity_),
      Comparator_(TCompare()) {
  uninitializedCopy(Data_, other);
}

template <typename T, typename TCompare>
THeap<T, TCompare>& THeap<T, TCompare>::operator=(
    const THeap<T, TCompare>& other) {
  if (this != &other) {
    TValueType* new_data = reinterpret_cast<TValueType*>(
        ::operator new(sizeof(TValueType) * other.Capacity_));
    uninitializedCopy(new_data, other);
    free(Data_, Size_);
    Data_ = new_data;
    Size_ = other.Size_;
    Capacity_ = other.Capacity_;
  }
  return *this;
}

template <typename T, typename TCompare>
THeap<T, TCompare>::THeap(THeap&& other) noexcept
    : Data_(nullptr),
      Size_(0),
      Capacity_(0),
      Comparator_(TCompare()) {
  swap(other);
}

template <typename T, typename TCompare>
THeap<T, TCompare>& THeap<T, TCompare>::operator=(
    THeap<T, TCompare>&& other) noexcept {
  if (this != &other) {
    free(Data_, Size_);
    Data_ = nullptr;
    Size_ = 0;
    Capacity_ = 0;
    swap(other);
  }
  return *this;
}

template <typename T, typename TCompare>
THeap<T, TCompare>::~THeap() {
  free(Data_, Size_);
}

// Element access
template <typename T, typename TCompare>
[[nodiscard]] typename THeap<T, TCompare>::TConstReference
THeap<T, TCompare>::top() const {
  if (Size_ == 0) {
    throw std::length_error("Heap is empty");
  }
  return Data_[0];
}

// Capacity
template <typename T, typename TCompare>
[[nodiscard]] bool THeap<T, TCompare>::empty() const noexcept {
  return Size_ == 0;
}

template <typename T, typename TCompare>
[[nodiscard]] typename THeap<T, TCompare>::TSizeType
THeap<T, TCompare>::getSize() const noexcept {
  return Size_;
}

// Modifiers
template <typename T, typename TCompare>
void THeap<T, TCompare>::push(const TValueType& value) {
  if (Size_ == Capacity_) {
    this->resize();
  }
  new (Data_ + Size_) TValueType(value);
  ++Size_;
  siftingUp(Size_ - 1);
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::push(const TValueType&& value) {
  if (Size_ == Capacity_) {
    this->resize();
  }
  new (Data_ + Size_) TValueType(std::move(value));
  ++Size_;
  siftingUp(Size_ - 1);
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::pop() {
  if (Size_ == 0) {
    throw std::length_error("Heap is empty");
  }
  Data_[0] = std::move(Data_[Size_ - 1]);
  if (!std::is_trivially_destructible_v<T>) {
    (Data_ + Size_ - 1)->~TValueType();
  }
  --Size_;
  siftingDown(0);
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::swap(THeap<T, TCompare>& other) noexcept {
  std::swap(Data_, other.Data_);
  std::swap(Size_, other.Size_);
  std::swap(Capacity_, other.Capacity_);
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::free(TValueType* data_to_free,
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

template <typename T, typename TCompare>
void THeap<T, TCompare>::uninitializedCopy(
    TValueType* copy_to, const THeap<T, TCompare>& copy_from) {
  TSizeType copied_objects = 0;
  try {
    for (; copied_objects < copy_from.Size_; ++copied_objects) {
      new (copy_to + copied_objects)
          TValueType(copy_from.Data_[copied_objects]);
    }
  } catch (...) {
    free(copy_to, copied_objects);
    throw;
  }
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::uninitializedCopy(TValueType* copy_to,
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

template <typename T, typename TCompare>
[[nodiscard]] typename THeap<T, TCompare>::TSizeType
THeap<T, TCompare>::getLeft(TSizeType index) const noexcept {
  return 2 * index + 1;
}

template <typename T, typename TCompare>
[[nodiscard]] typename THeap<T, TCompare>::TSizeType
THeap<T, TCompare>::getRight(TSizeType index) const noexcept {
  return 2 * index + 2;
}

template <typename T, typename TCompare>
[[nodiscard]] typename THeap<T, TCompare>::TSizeType
THeap<T, TCompare>::getParent(TSizeType index) const noexcept {
  return (index - 1) / 2;
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::resize() {
  TSizeType new_capacity = Capacity_ > 0 ? Capacity_ * 2 : Capacity_ + 1;
  TValueType* new_data = reinterpret_cast<TValueType*>(
      ::operator new(sizeof(TValueType) * new_capacity));
  if (std::is_move_constructible_v<TValueType>) {
    TSizeType moved_objects = 0;
    while (moved_objects < Size_) {
      new (new_data + moved_objects)
          TValueType(std::move(Data_[moved_objects]));
      ++moved_objects;
    }
    ::operator delete(Data_);
  } else {
    uninitializedCopy(new_data, *this);
    free(Data_, Size_);
  }
  Data_ = new_data;
  Capacity_ = new_capacity;
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::siftingDown(TSizeType index) noexcept(
    std::is_nothrow_swappable_v<TValueType>) {
  TSizeType iter_start_index = index;
  TSizeType iter_end_index = index;
  do {
    iter_start_index = iter_end_index;
    TSizeType left = getLeft(iter_start_index);
    TSizeType right = getRight(iter_start_index);
    if ((left < Size_) && (comparator_(Data_[left], Data_[iter_end_index]))) {
      iter_end_index = left;
    }
    if ((right < Size_) && (comparator_(Data_[right], Data_[iter_end_index]))) {
      iter_end_index = right;
    }
    if (iter_start_index != iter_end_index) {
      std::swap(Data_[iter_start_index], Data_[iter_end_index]);
    }
  } while (iter_start_index != iter_end_index);
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::siftingUp(TSizeType index) noexcept(
    std::is_nothrow_swappable_v<TValueType>) {
  bool is_sifting_complete = false;
  while ((index > 0) && !is_sifting_complete) {
    is_sifting_complete = true;
    TSizeType parent = getParent(index);
    if (Comparator_(Data_[index], Data_[parent])) {
      std::swap(Data_[index], Data_[parent]);
      index = parent;
      is_sifting_complete = false;
    }
  }
}

template <typename T, typename TCompare>
void THeap<T, TCompare>::makeHeap() noexcept(std::is_nothrow_swappable_v<T>) {
  for (TSizeType i = (Size_ / 2 - 1); i > 0; --i) {
    siftingDown(i);
  }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NHeap
