#pragma once

#include <cstddef>

namespace NAds::NDs::NQueue {

////////////////////////////////////////////////////////////////////////////////

// Queue implementation on a ring buffer
// If N > 0 then allocate memory on stack
template <typename T, std::size_t N>
class TQueue {
public:
  using TReference = T&;
  using TConstReference = const T&;

  TQueue() noexcept;

  TQueue(const TQueue<T, N>& other);

  TQueue& operator=(const TQueue<T, N>& other);

  TQueue(TQueue<T, N>&& other) noexcept;

  TQueue& operator=(TQueue<T, N>&& other) noexcept;

  ~TQueue();

  // Element access
  [[nodiscard]] TReference front();

  [[nodiscard]] TConstReference front() const;

  [[nodiscard]] TReference back();

  [[nodiscard]] TConstReference back() const;

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
class TQueue<T, 0> {
public:
  using TReference = T&;
  using TConstReference = const T&;

  explicit TQueue<T, 0>(std::size_t capacity);

  TQueue(const TQueue<T, 0>& other);

  TQueue& operator=(const TQueue<T, 0>& other);

  TQueue(TQueue<T, 0>&& other) noexcept;

  TQueue& operator=(TQueue<T, 0>&& other) noexcept;

  ~TQueue();

  // Element access
  [[nodiscard]] TReference front();

  [[nodiscard]] TConstReference front() const;

  [[nodiscard]] TReference back();

  [[nodiscard]] TConstReference back() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] std::size_t getSize() const noexcept;

  // Modifiers
  void pop();

  void push(const T& value);

  void push(T&& value);

private:
  void swap(TQueue<T, 0>& other) noexcept;

  void free(std::size_t destructor_calls) noexcept;

  void uninitializedCopy(const TQueue<T, 0>& other);

  void resize();

  T* data_;
  std::size_t size_;
  std::size_t capacity_;
  std::size_t top_pointer_;
  std::size_t back_pointer_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NQueue

#define ADS_DS_QUEUE_QUEUE_INL_HPP_
#include "queue-inl.hpp"
#undef ADS_DS_QUEUE_QUEUE_INL_HPP_
