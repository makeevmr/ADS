#pragma once

#include <cstddef>

#include "min_max_node.hpp"

namespace ads::ds::min_max_queue {

////////////////////////////////////////////////////////////////////////////////

// Stack implementation with T* dynamic array
template <typename T>
class Stack {
public:
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  explicit Stack(const size_type& capacity = 100);

  Stack(const Stack<T>& other);

  Stack& operator=(const Stack<T>& other);

  Stack(Stack<T>&& other) noexcept;

  Stack& operator=(Stack<T>&& other) noexcept;

  ~Stack();

  // Element access
  [[nodiscard]] reference top();

  [[nodiscard]] const_reference top() const;

  // Necessary for efficient implementation of a queue on 2 stacks
  // TODO: consider refactor this method (maybe make it private)
  [[nodiscard]] reference bottom();

  [[nodiscard]] const_reference bottom() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] size_type getSize() const noexcept;

  // Modifiers
  void resize(size_type new_capacity);

  void pop();

  void push(const T& value);

  void push(const T&& value);

  // Move content of one push_stack to pop_stack
  template <typename Type>
  class MoveStackContent {
  public:
    static void get(Stack<Type>& move_to, Stack<Type>& move_from);
  };

private:
  void swap(Stack<T>& other) noexcept;

  static void free(T* data_to_free, size_type destructor_calls) noexcept;

  static void uninitializedCopy(T* copy_to, const Stack<T>& copy_from);

  T* data_;
  size_type size_;
  size_type capacity_;
};

////////////////////////////////////////////////////////////////////////////////

// Specialization for a problem condition
template <>
template <>
class Stack<MinMaxNode>::MoveStackContent<MinMaxNode> {
public:
  static void get(Stack<MinMaxNode>& move_to, Stack<MinMaxNode>& move_from);
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::min_max_queue

#define ADS_DS_MIN_MAX_QUEUE_STACK_INL_HPP_
#include "stack-inl.hpp"
#undef ADS_DS_MIN_MAX_QUEUE_STACK_INL_HPP_
