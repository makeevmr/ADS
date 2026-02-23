#pragma once

#include "stack.hpp"

namespace ads::ds::min_max_queue {

////////////////////////////////////////////////////////////////////////////////

// Queue implementation using two stacks
template <typename T>
class Queue {
public:
  using reference = T&;
  using const_reference = const T&;
  using size_type = std::size_t;

  explicit Queue(const size_type& capacity);

  Queue(const Queue<T>& other);

  Queue& operator=(const Queue<T>& other);

  Queue(Queue<T>&& other) noexcept;

  Queue& operator=(Queue<T>&& other) noexcept;

  ~Queue();

  // Element access
  [[nodiscard]] reference front();

  [[nodiscard]] const_reference front() const;

  [[nodiscard]] reference back();

  [[nodiscard]] const_reference back() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] std::size_t getSize() const noexcept;

  // Modifiers
  void pop();

  void push(const T& value);

  void push(T&& value);

private:
  size_type size_;
  Stack<T> push_stack_;
  Stack<T> pop_stack_;
};

////////////////////////////////////////////////////////////////////////////////

template <>
class Queue<MinMaxNode> {
public:
  using reference = MinMaxNode&;
  using const_reference = const MinMaxNode&;
  using size_type = std::size_t;

  explicit Queue(const size_type& capacity);

  Queue(const Queue<MinMaxNode>& other);

  Queue& operator=(const Queue<MinMaxNode>& other);

  Queue(Queue<MinMaxNode>&& other) noexcept;

  Queue& operator=(Queue<MinMaxNode>&& other) noexcept;

  ~Queue();

  // Element access
  [[nodiscard]] reference front();

  [[nodiscard]] const_reference front() const;

  [[nodiscard]] reference back();

  [[nodiscard]] const_reference back() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] std::size_t getSize() const noexcept;

  // Requests
  [[nodiscard]] int getMaxDiff() const;

  // Modifiers
  void pop();

  void push(int value);

  void resize(size_type new_capacity);

private:
  size_type size_;
  Stack<MinMaxNode> push_stack_;
  Stack<MinMaxNode> pop_stack_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::min_max_queue

#define ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
#include "queue-inl.hpp"
#undef ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
