#ifndef ADS_DS_MIN_MAX_QUEUE_STACK_INL_HPP_
#error "Direct inclusion of this file is not allowed, include stack.hpp"
// For the sake of sane code completion.
#include "stack.hpp"
#endif

#include <stdexcept>
#include <type_traits>

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
TStack<T>::TStack(const TSizeType& capacity)
    : data_(reinterpret_cast<T*>(::operator new(sizeof(T) * capacity))),
      size_(0),
      capacity_(capacity) {}

template <typename T>
TStack<T>::TStack(const TStack<T>& other)
    : data_(reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity_))),
      size_(other.size_),
      capacity_(other.capacity_) {
  uninitializedCopy(data_, other);
}

template <typename T>
TStack<T>::TStack& TStack<T>::operator=(const TStack<T>& other) {
  if (this != &other) {
    T* new_data =
        reinterpret_cast<T*>(::operator new(sizeof(T) * other.capacity_));
    uninitializedCopy(new_data, other);
    free(data_, size_);
    data_ = new_data;
    size_ = other.size_;
    capacity_ = other.capacity_;
  }
  return *this;
}

template <typename T>
TStack<T>::TStack(TStack<T>&& other) noexcept
    : data_(nullptr),
      size_(0),
      capacity_(0) {
  swap(other);
}

template <typename T>
TStack<T>::TStack& TStack<T>::operator=(TStack<T>&& other) noexcept {
  if (this != &other) {
    free(data_, size_);
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
    swap(other);
  }
  return *this;
}

template <typename T>
TStack<T>::~TStack() {
  free(data_, size_);
}

template <typename T>
[[nodiscard]] TStack<T>::TReference TStack<T>::top() {
  if (size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return data_[size_ - 1];
}

template <typename T>
[[nodiscard]] TStack<T>::TConstReference TStack<T>::top() const {
  if (size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return data_[size_ - 1];
}

template <typename T>
[[nodiscard]] TStack<T>::TReference TStack<T>::bottom() {
  if (size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return data_[0];
}

template <typename T>
[[nodiscard]] TStack<T>::TConstReference TStack<T>::bottom() const {
  if (size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return data_[0];
}

template <typename T>
[[nodiscard]] bool TStack<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
[[nodiscard]] TStack<T>::TSizeType TStack<T>::getSize() const noexcept {
  return size_;
}

template <typename T>
void TStack<T>::resize(TSizeType new_capacity) {
  T* new_data = reinterpret_cast<T*>(::operator new(sizeof(T) * new_capacity));
  if (std::is_move_constructible_v<T>) {
    TSizeType copied_objects = 0;
    while (copied_objects < size_) {
      new (new_data + copied_objects) T(std::move(data_[copied_objects]));
      ++copied_objects;
    }
    ::operator delete(data_);
  } else {
    uninitializedCopy(new_data, *this);
    free(data_, size_);
  }
  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T>
void TStack<T>::pop() {
  if (size_ == 0) {
    throw std::length_error("Empty stack");
  }
  if (!std::is_trivially_destructible_v<T>) {
    (data_ + size_ - 1)->~T();
  }
  --size_;
}

template <typename T>
void TStack<T>::push(const T& value) {
  if (size_ == capacity_) {
    resize(capacity_ * 2 + 1);
  }
  new (data_ + size_) T(value);
  ++size_;
}

template <typename T>
void TStack<T>::push(const T&& value) {
  if (size_ == capacity_) {
    resize(capacity_ * 2 + 1);
  }
  new (data_ + size_) T(std::move(value));
  ++size_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename TType>
void TStack<T>::TMoveStackContent<TType>::get(TStack<TType>& move_to,
                                              TStack<TType>& move_from) {
  if (std::is_move_constructible_v<TType>) {
    while (!move_from.empty()) {
      move_to.push(std::move(move_from.top()));
      --move_from.size_;
    }
  } else {
    while (!move_from.empty()) {
      move_to.push(move_from.top());
      move_from.pop();
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
void TStack<T>::swap(TStack<T>& other) noexcept {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
void TStack<T>::free(T* data_to_free, TSizeType destructor_calls) noexcept {
  if (!std::is_trivially_destructible_v<T>) {
    TSizeType destroyed_objects = 0;
    while (destroyed_objects < destructor_calls) {
      (data_to_free + destroyed_objects)->~T();
      ++destroyed_objects;
    }
  }
  ::operator delete(data_to_free);
}

template <typename T>
void TStack<T>::uninitializedCopy(T* copy_to, const TStack<T>& copy_from) {
  TSizeType copied_objects = 0;
  try {
    for (; copied_objects < copy_from.size_; ++copied_objects) {
      new (copy_to + copied_objects) T(copy_from.data_[copied_objects]);
    }
  } catch (...) {
    free(copy_to, copied_objects);
    throw;
  }
}

////////////////////////////////////////////////////////////////////////////////

// Specialization for a problem condition
void TStack<TMinMaxNode>::TMoveStackContent<TMinMaxNode>::get(
    TStack<TMinMaxNode>& move_to, TStack<TMinMaxNode>& move_from) {
  while (!move_from.empty()) {
    if (move_to.empty()) {
      TMinMaxNode& move_from_top = move_from.top();
      move_to.push(TMinMaxNode{move_from_top.self_value,
                               move_from_top.self_value,
                               move_from_top.self_value});
    } else {
      TMinMaxNode& move_to_top = move_to.top();
      TMinMaxNode& move_from_top = move_from.top();
      move_to.push(TMinMaxNode{move_from_top.self_value,
                               move_to_top.min_value < move_from_top.self_value
                                   ? move_to_top.min_value
                                   : move_from_top.self_value,
                               move_to_top.max_value > move_from_top.self_value
                                   ? move_to_top.max_value
                                   : move_from_top.self_value});
    }
    move_from.pop();
  }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
