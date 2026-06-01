#pragma once

#include <cstddef>

#include "min_max_node.hpp"

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

// Stack implementation with T* dynamic array
template <typename T>
class TStack {
public:
  using TReference = T&;
  using TConstReference = const T&;
  using TSizeType = std::size_t;

  explicit TStack(const TSizeType& capacity = 100);

  TStack(const TStack<T>& other);

  TStack& operator=(const TStack<T>& other);

  TStack(TStack<T>&& other) noexcept;

  TStack& operator=(TStack<T>&& other) noexcept;

  ~TStack();

  // Element access
  [[nodiscard]] TReference top();

  [[nodiscard]] TConstReference top() const;

  // Necessary for efficient implementation of a queue on 2 stacks
  // TODO: consider refactor this method (maybe make it private)
  [[nodiscard]] TReference bottom();

  [[nodiscard]] TConstReference bottom() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] TSizeType getSize() const noexcept;

  // Modifiers
  void resize(TSizeType new_capacity);

  void pop();

  void push(const T& value);

  void push(const T&& value);

  // Move content of one push_stack to pop_stack
  template <typename Type>
  class TMoveStackContent {
  public:
    static void get(TStack<Type>& move_to, TStack<Type>& move_from);
  };

private:
  void swap(TStack<T>& other) noexcept;

  static void free(T* data_to_free, TSizeType destructor_calls) noexcept;

  static void uninitializedCopy(T* copy_to, const TStack<T>& copy_from);

  T* data_;
  TSizeType size_;
  TSizeType capacity_;
};

////////////////////////////////////////////////////////////////////////////////

// Specialization for a problem condition
template <>
template <>
class TStack<TMinMaxNode>::TMoveStackContent<TMinMaxNode> {
public:
  static void get(TStack<TMinMaxNode>& move_to, TStack<TMinMaxNode>& move_from);
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue

#define ADS_DS_MIN_MAX_QUEUE_STACK_INL_HPP_
#include "stack-inl.hpp"
#undef ADS_DS_MIN_MAX_QUEUE_STACK_INL_HPP_
