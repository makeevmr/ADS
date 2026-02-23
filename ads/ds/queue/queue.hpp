#pragma once

#include <cstddef>

namespace ads::ds::queue {

////////////////////////////////////////////////////////////////////////////////

// Queue implementation on a ring buffer
// If N > 0 then allocate memory on stack
template <typename T, std::size_t N>
class Queue {
public:
  using reference = T&;
  using const_reference = const T&;

  Queue() noexcept;

  Queue(const Queue<T, N>& other);

  Queue& operator=(const Queue<T, N>& other);

  Queue(Queue<T, N>&& other) noexcept;

  Queue& operator=(Queue<T, N>&& other) noexcept;

  ~Queue();

  // Element access
  [[nodiscard]] reference front();

  [[nodiscard]] const_reference front() const;

  [[nodiscard]] reference back();

  [[nodiscard]] const_reference back() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] std::size_t getSize() const noexcept;

  void pop();

  void push(const T& value);

  void push(T&& value);

private:
  T data_[N];
  std::size_t size_;
  std::size_t top_pointer_;
  std::size_t back_pointer_;
};

////////////////////////////////////////////////////////////////////////////////

// If N == 0 then allocate memory on heap
template <typename T>
class Queue<T, 0> {
public:
  using reference = T&;
  using const_reference = const T&;

  explicit Queue<T, 0>(std::size_t capacity);

  Queue(const Queue<T, 0>& other);

  Queue& operator=(const Queue<T, 0>& other);

  Queue(Queue<T, 0>&& other) noexcept;

  Queue& operator=(Queue<T, 0>&& other) noexcept;

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
  void swap(Queue<T, 0>& other) noexcept;

  void free(std::size_t destructor_calls) noexcept;

  void uninitializedCopy(const Queue<T, 0>& other);

  void resize();

  T* data_;
  std::size_t size_;
  std::size_t capacity_;
  std::size_t top_pointer_;
  std::size_t back_pointer_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::queue

#define ADS_DS_QUEUE_QUEUE_INL_HPP_
#include "queue-inl.hpp"
#undef ADS_DS_QUEUE_QUEUE_INL_HPP_
