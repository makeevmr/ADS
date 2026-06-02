#ifndef ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
#error "Direct inclusion of this file is not allowed, include queue.hpp"
// For the sake of sane code completion.
#include "queue.hpp"
#endif

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
TQueue<T>::TQueue(const TSizeType& capacity)
    : Size_(0),
      PushStack_(TStack<T>(capacity)),
      PopStack_(TStack<T>(capacity)) {}

template <typename T>
TQueue<T>::TQueue(const TQueue<T>& other) = default;

template <typename T>
TQueue<T>::TQueue& TQueue<T>::operator=(const TQueue<T>& other) = default;

template <typename T>
TQueue<T>::TQueue(TQueue<T>&& other) noexcept = default;

template <typename T>
TQueue<T>::TQueue& TQueue<T>::operator=(TQueue<T>&& other) noexcept = default;

template <typename T>
TQueue<T>::~TQueue() = default;

template <typename T>
[[nodiscard]] TQueue<T>::TReference TQueue<T>::front() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PopStack_.empty()) {
    return PushStack_.bottom();
  }
  return PopStack_.top();
}

template <typename T>
[[nodiscard]] TQueue<T>::TConstReference TQueue<T>::front() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PopStack_.empty()) {
    return PushStack_.bottom();
  }
  return PopStack_.top();
}

template <typename T>
[[nodiscard]] TQueue<T>::TReference TQueue<T>::back() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!PushStack_.empty()) {
    return PushStack_.top();
  }
  return PopStack_.bottom();
}

template <typename T>
[[nodiscard]] TQueue<T>::TConstReference TQueue<T>::back() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!PushStack_.empty()) {
    return PushStack_.top();
  }
  return PopStack_.bottom();
}

template <typename T>
// Capacity
[[nodiscard]] bool TQueue<T>::empty() const noexcept {
  return Size_ == 0;
}

template <typename T>
[[nodiscard]] std::size_t TQueue<T>::getSize() const noexcept {
  return Size_;
}

template <typename T>
// Modifiers
void TQueue<T>::pop() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PopStack_.empty()) {
    TStack<T>::template MoveStackContent<T>::get(PopStack_, PushStack_);
  }
  PopStack_.pop();
  --Size_;
}

template <typename T>
void TQueue<T>::push(const T& value) {
  PushStack_.push(value);
  ++Size_;
}

template <typename T>
void TQueue<T>::push(T&& value) {
  PushStack_.push(value);
  ++Size_;
}

////////////////////////////////////////////////////////////////////////////////

TQueue<TMinMaxNode>::TQueue(const TSizeType& capacity)
    : Size_(0),
      PushStack_(TStack<TMinMaxNode>(capacity)),
      PopStack_(TStack<TMinMaxNode>(capacity)) {}

TQueue<TMinMaxNode>::TQueue(const TQueue<TMinMaxNode>& other) = default;

TQueue<TMinMaxNode>& TQueue<TMinMaxNode>::operator=(
    const TQueue<TMinMaxNode>& other) = default;

TQueue<TMinMaxNode>::TQueue(TQueue<TMinMaxNode>&& other) noexcept = default;

TQueue<TMinMaxNode>& TQueue<TMinMaxNode>::operator=(
    TQueue<TMinMaxNode>&& other) noexcept = default;

TQueue<TMinMaxNode>::~TQueue() = default;

// Element access
[[nodiscard]] TQueue<TMinMaxNode>::TReference TQueue<TMinMaxNode>::front() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PopStack_.empty()) {
    return PushStack_.bottom();
  }
  return PopStack_.top();
}

[[nodiscard]] TQueue<TMinMaxNode>::TConstReference TQueue<TMinMaxNode>::front()
    const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PopStack_.empty()) {
    return PushStack_.bottom();
  }
  return PopStack_.top();
}

[[nodiscard]] TQueue<TMinMaxNode>::TReference TQueue<TMinMaxNode>::back() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!PushStack_.empty()) {
    return PushStack_.top();
  }
  return PopStack_.bottom();
}

[[nodiscard]] TQueue<TMinMaxNode>::TConstReference TQueue<TMinMaxNode>::back()
    const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!PushStack_.empty()) {
    return PushStack_.top();
  }
  return PopStack_.bottom();
}

// Capacity
[[nodiscard]] bool TQueue<TMinMaxNode>::empty() const noexcept {
  return Size_ == 0;
}

[[nodiscard]] std::size_t TQueue<TMinMaxNode>::getSize() const noexcept {
  return Size_;
}

// Requests
[[nodiscard]] int TQueue<TMinMaxNode>::getMaxDiff() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PushStack_.empty()) {
    const TMinMaxNode& pop_stack_top = PopStack_.top();
    return pop_stack_top.MaxValue - pop_stack_top.MinValue;
  }
  if (PopStack_.empty()) {
    const TMinMaxNode& push_stack_top = PushStack_.top();
    return push_stack_top.MaxValue - push_stack_top.MinValue;
  }
  const TMinMaxNode& pop_stack_top = PopStack_.top();
  const TMinMaxNode& push_stack_top = PushStack_.top();
  int max_value = push_stack_top.MaxValue > pop_stack_top.MaxValue
                      ? push_stack_top.MaxValue
                      : pop_stack_top.MaxValue;
  int min_value = push_stack_top.MinValue < pop_stack_top.MinValue
                      ? push_stack_top.MinValue
                      : pop_stack_top.MinValue;
  return max_value - min_value;
}

// Modifiers
void TQueue<TMinMaxNode>::pop() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (PopStack_.empty()) {
    TStack<TMinMaxNode>::template TMoveStackContent<TMinMaxNode>::get(
        PopStack_, PushStack_);
  }
  PopStack_.pop();
  --Size_;
}

void TQueue<TMinMaxNode>::push(int value) {
  if (PushStack_.empty()) {
    PushStack_.push(TMinMaxNode{value, value, value});
  } else {
    TMinMaxNode&& node_to_push = {value, value, value};
    const TMinMaxNode& top_node = PushStack_.top();
    if (top_node.MinValue < node_to_push.MinValue) {
      node_to_push.MinValue = top_node.MinValue;
    }
    if (top_node.MaxValue > node_to_push.MaxValue) {
      node_to_push.MaxValue = top_node.MaxValue;
    }
    PushStack_.push(node_to_push);
  }
  ++Size_;
}

void TQueue<TMinMaxNode>::resize(TSizeType new_capacity) {
  PopStack_.resize(new_capacity);
  PushStack_.resize(new_capacity);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
