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
    : Data_(reinterpret_cast<T*>(::operator new(sizeof(T) * capacity))),
      Size_(0),
      Capacity_(capacity) {}

template <typename T>
TStack<T>::TStack(const TStack<T>& other)
    : Data_(reinterpret_cast<T*>(::operator new(sizeof(T) * other.Capacity_))),
      Size_(other.Size_),
      Capacity_(other.Capacity_) {
  uninitializedCopy(Data_, other);
}

template <typename T>
TStack<T>::TStack& TStack<T>::operator=(const TStack<T>& other) {
  if (this != &other) {
    T* new_data =
        reinterpret_cast<T*>(::operator new(sizeof(T) * other.Capacity_));
    uninitializedCopy(new_data, other);
    free(Data_, Size_);
    Data_ = new_data;
    Size_ = other.Size_;
    Capacity_ = other.Capacity_;
  }
  return *this;
}

template <typename T>
TStack<T>::TStack(TStack<T>&& other) noexcept
    : Data_(nullptr),
      Size_(0),
      Capacity_(0) {
  swap(other);
}

template <typename T>
TStack<T>::TStack& TStack<T>::operator=(TStack<T>&& other) noexcept {
  if (this != &other) {
    free(Data_, Size_);
    Data_ = nullptr;
    Size_ = 0;
    Capacity_ = 0;
    swap(other);
  }
  return *this;
}

template <typename T>
TStack<T>::~TStack() {
  free(Data_, Size_);
}

template <typename T>
[[nodiscard]] TStack<T>::TReference TStack<T>::top() {
  if (Size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return Data_[Size_ - 1];
}

template <typename T>
[[nodiscard]] TStack<T>::TConstReference TStack<T>::top() const {
  if (Size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return Data_[Size_ - 1];
}

template <typename T>
[[nodiscard]] TStack<T>::TReference TStack<T>::bottom() {
  if (Size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return Data_[0];
}

template <typename T>
[[nodiscard]] TStack<T>::TConstReference TStack<T>::bottom() const {
  if (Size_ == 0) {
    throw std::length_error("Empty stack");
  }
  return Data_[0];
}

template <typename T>
[[nodiscard]] bool TStack<T>::empty() const noexcept {
  return Size_ == 0;
}

template <typename T>
[[nodiscard]] TStack<T>::TSizeType TStack<T>::getSize() const noexcept {
  return Size_;
}

template <typename T>
void TStack<T>::resize(TSizeType new_capacity) {
  T* new_data = reinterpret_cast<T*>(::operator new(sizeof(T) * new_capacity));
  if (std::is_move_constructible_v<T>) {
    TSizeType copied_objects = 0;
    while (copied_objects < Size_) {
      new (new_data + copied_objects) T(std::move(Data_[copied_objects]));
      ++copied_objects;
    }
    ::operator delete(Data_);
  } else {
    uninitializedCopy(new_data, *this);
    free(Data_, Size_);
  }
  Data_ = new_data;
  Capacity_ = new_capacity;
}

template <typename T>
void TStack<T>::pop() {
  if (Size_ == 0) {
    throw std::length_error("Empty stack");
  }
  if (!std::is_trivially_destructible_v<T>) {
    (Data_ + Size_ - 1)->~T();
  }
  --Size_;
}

template <typename T>
void TStack<T>::push(const T& value) {
  if (Size_ == Capacity_) {
    resize(Capacity_ * 2 + 1);
  }
  new (Data_ + Size_) T(value);
  ++Size_;
}

template <typename T>
void TStack<T>::push(const T&& value) {
  if (Size_ == Capacity_) {
    resize(Capacity_ * 2 + 1);
  }
  new (Data_ + Size_) T(std::move(value));
  ++Size_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename TType>
void TStack<T>::TMoveStackContent<TType>::get(TStack<TType>& move_to,
                                              TStack<TType>& move_from) {
  if (std::is_move_constructible_v<TType>) {
    while (!move_from.empty()) {
      move_to.push(std::move(move_from.top()));
      --move_from.Size_;
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
  std::swap(Data_, other.Data_);
  std::swap(Size_, other.Size_);
  std::swap(Capacity_, other.Capacity_);
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
    for (; copied_objects < copy_from.Size_; ++copied_objects) {
      new (copy_to + copied_objects) T(copy_from.Data_[copied_objects]);
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
      move_to.push(TMinMaxNode{move_from_top.SelfValue, move_from_top.SelfValue,
                               move_from_top.SelfValue});
    } else {
      TMinMaxNode& move_to_top = move_to.top();
      TMinMaxNode& move_from_top = move_from.top();
      move_to.push(TMinMaxNode{move_from_top.SelfValue,
                               move_to_top.MinValue < move_from_top.SelfValue
                                   ? move_to_top.MinValue
                                   : move_from_top.SelfValue,
                               move_to_top.MaxValue > move_from_top.SelfValue
                                   ? move_to_top.MaxValue
                                   : move_from_top.SelfValue});
    }
    move_from.pop();
  }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
