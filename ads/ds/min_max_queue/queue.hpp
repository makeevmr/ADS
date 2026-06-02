#pragma once

#include "stack.hpp"

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

// Queue implementation using two stacks
template <typename T>
class TQueue {
public:
  using TReference = T&;
  using TConstReference = const T&;
  using TSizeType = std::size_t;

  explicit TQueue(const TSizeType& capacity);

  TQueue(const TQueue<T>& other);

  TQueue& operator=(const TQueue<T>& other);

  TQueue(TQueue<T>&& other) noexcept;

  TQueue& operator=(TQueue<T>&& other) noexcept;

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
  TSizeType Size_;
  TStack<T> PushStack_;
  TStack<T> PopStack_;
};

////////////////////////////////////////////////////////////////////////////////

template <>
class TQueue<TMinMaxNode> {
public:
  using TReference = TMinMaxNode&;
  using TConstReference = const TMinMaxNode&;
  using TSizeType = std::size_t;

  explicit TQueue(const TSizeType& capacity);

  TQueue(const TQueue<TMinMaxNode>& other);

  TQueue& operator=(const TQueue<TMinMaxNode>& other);

  TQueue(TQueue<TMinMaxNode>&& other) noexcept;

  TQueue& operator=(TQueue<TMinMaxNode>&& other) noexcept;

  ~TQueue();

  // Element access
  [[nodiscard]] TReference front();

  [[nodiscard]] TConstReference front() const;

  [[nodiscard]] TReference back();

  [[nodiscard]] TConstReference back() const;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] std::size_t getSize() const noexcept;

  // Requests
  [[nodiscard]] int getMaxDiff() const;

  // Modifiers
  void pop();

  void push(int value);

  void resize(TSizeType new_capacity);

private:
  TSizeType Size_;
  TStack<TMinMaxNode> PushStack_;
  TStack<TMinMaxNode> PopStack_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue

#define ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
#include "queue-inl.hpp"
#undef ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
