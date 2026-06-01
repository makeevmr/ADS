#ifndef ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
#error "Direct inclusion of this file is not allowed, include queue.hpp"
// For the sake of sane code completion.
#include "queue.hpp"
#endif

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
TQueue<T>::TQueue(const TSizeType& capacity)
    : size_(0),
      push_stack_(TStack<T>(capacity)),
      pop_stack_(TStack<T>(capacity)) {}

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
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

template <typename T>
[[nodiscard]] TQueue<T>::TConstReference TQueue<T>::front() const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

template <typename T>
[[nodiscard]] TQueue<T>::TReference TQueue<T>::back() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!push_stack_.empty()) {
    return push_stack_.top();
  }
  return pop_stack_.bottom();
}

template <typename T>
[[nodiscard]] TQueue<T>::TConstReference TQueue<T>::back() const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!push_stack_.empty()) {
    return push_stack_.top();
  }
  return pop_stack_.bottom();
}

template <typename T>
// Capacity
[[nodiscard]] bool TQueue<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
[[nodiscard]] std::size_t TQueue<T>::getSize() const noexcept {
  return size_;
}

template <typename T>
// Modifiers
void TQueue<T>::pop() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    TStack<T>::template MoveStackContent<T>::get(pop_stack_, push_stack_);
  }
  pop_stack_.pop();
  --size_;
}

template <typename T>
void TQueue<T>::push(const T& value) {
  push_stack_.push(value);
  ++size_;
}

template <typename T>
void TQueue<T>::push(T&& value) {
  push_stack_.push(value);
  ++size_;
}

////////////////////////////////////////////////////////////////////////////////

TQueue<TMinMaxNode>::TQueue(const TSizeType& capacity)
    : size_(0),
      push_stack_(TStack<TMinMaxNode>(capacity)),
      pop_stack_(TStack<TMinMaxNode>(capacity)) {}

TQueue<TMinMaxNode>::TQueue(const TQueue<TMinMaxNode>& other) = default;

TQueue<TMinMaxNode>& TQueue<TMinMaxNode>::operator=(
    const TQueue<TMinMaxNode>& other) = default;

TQueue<TMinMaxNode>::TQueue(TQueue<TMinMaxNode>&& other) noexcept = default;

TQueue<TMinMaxNode>& TQueue<TMinMaxNode>::operator=(
    TQueue<TMinMaxNode>&& other) noexcept = default;

TQueue<TMinMaxNode>::~TQueue() = default;

// Element access
[[nodiscard]] TQueue<TMinMaxNode>::TReference TQueue<TMinMaxNode>::front() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

[[nodiscard]] TQueue<TMinMaxNode>::TConstReference TQueue<TMinMaxNode>::front()
    const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

[[nodiscard]] TQueue<TMinMaxNode>::TReference TQueue<TMinMaxNode>::back() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!push_stack_.empty()) {
    return push_stack_.top();
  }
  return pop_stack_.bottom();
}

[[nodiscard]] TQueue<TMinMaxNode>::TConstReference TQueue<TMinMaxNode>::back()
    const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!push_stack_.empty()) {
    return push_stack_.top();
  }
  return pop_stack_.bottom();
}

// Capacity
[[nodiscard]] bool TQueue<TMinMaxNode>::empty() const noexcept {
  return size_ == 0;
}

[[nodiscard]] std::size_t TQueue<TMinMaxNode>::getSize() const noexcept {
  return size_;
}

// Requests
[[nodiscard]] int TQueue<TMinMaxNode>::getMaxDiff() const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (push_stack_.empty()) {
    const TMinMaxNode& pop_stack_top = pop_stack_.top();
    return pop_stack_top.max_value - pop_stack_top.min_value;
  }
  if (pop_stack_.empty()) {
    const TMinMaxNode& push_stack_top = push_stack_.top();
    return push_stack_top.max_value - push_stack_top.min_value;
  }
  const TMinMaxNode& pop_stack_top = pop_stack_.top();
  const TMinMaxNode& push_stack_top = push_stack_.top();
  int max_value = push_stack_top.max_value > pop_stack_top.max_value
                      ? push_stack_top.max_value
                      : pop_stack_top.max_value;
  int min_value = push_stack_top.min_value < pop_stack_top.min_value
                      ? push_stack_top.min_value
                      : pop_stack_top.min_value;
  return max_value - min_value;
}

// Modifiers
void TQueue<TMinMaxNode>::pop() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    TStack<TMinMaxNode>::template TMoveStackContent<TMinMaxNode>::get(
        pop_stack_, push_stack_);
  }
  pop_stack_.pop();
  --size_;
}

void TQueue<TMinMaxNode>::push(int value) {
  if (push_stack_.empty()) {
    push_stack_.push(TMinMaxNode{value, value, value});
  } else {
    TMinMaxNode&& node_to_push = {value, value, value};
    const TMinMaxNode& top_node = push_stack_.top();
    if (top_node.min_value < node_to_push.min_value) {
      node_to_push.min_value = top_node.min_value;
    }
    if (top_node.max_value > node_to_push.max_value) {
      node_to_push.max_value = top_node.max_value;
    }
    push_stack_.push(node_to_push);
  }
  ++size_;
}

void TQueue<TMinMaxNode>::resize(TSizeType new_capacity) {
  pop_stack_.resize(new_capacity);
  push_stack_.resize(new_capacity);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
