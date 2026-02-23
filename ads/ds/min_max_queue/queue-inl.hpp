#ifndef ADS_DS_MIN_MAX_QUEUE_QUEUE_INL_HPP_
#error "Direct inclusion of this file is not allowed, include queue.hpp"
// For the sake of sane code completion.
#include "queue.hpp"
#endif

namespace ads::ds::min_max_queue {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T>::Queue(const size_type& capacity)
    : size_(0),
      push_stack_(Stack<T>(capacity)),
      pop_stack_(Stack<T>(capacity)) {}

template <typename T>
Queue<T>::Queue(const Queue<T>& other) = default;

template <typename T>
Queue<T>::Queue& Queue<T>::operator=(const Queue<T>& other) = default;

template <typename T>
Queue<T>::Queue(Queue<T>&& other) noexcept = default;

template <typename T>
Queue<T>::Queue& Queue<T>::operator=(Queue<T>&& other) noexcept = default;

template <typename T>
Queue<T>::~Queue() = default;

template <typename T>
[[nodiscard]] Queue<T>::reference Queue<T>::front() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

template <typename T>
[[nodiscard]] Queue<T>::const_reference Queue<T>::front() const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

template <typename T>
[[nodiscard]] Queue<T>::reference Queue<T>::back() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!push_stack_.empty()) {
    return push_stack_.top();
  }
  return pop_stack_.bottom();
}

template <typename T>
[[nodiscard]] Queue<T>::const_reference Queue<T>::back() const {
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
[[nodiscard]] bool Queue<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
[[nodiscard]] std::size_t Queue<T>::getSize() const noexcept {
  return size_;
}

template <typename T>
// Modifiers
void Queue<T>::pop() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    Stack<T>::template MoveStackContent<T>::get(pop_stack_, push_stack_);
  }
  pop_stack_.pop();
  --size_;
}

template <typename T>
void Queue<T>::push(const T& value) {
  push_stack_.push(value);
  ++size_;
}

template <typename T>
void Queue<T>::push(T&& value) {
  push_stack_.push(value);
  ++size_;
}

////////////////////////////////////////////////////////////////////////////////

Queue<MinMaxNode>::Queue(const size_type& capacity)
    : size_(0),
      push_stack_(Stack<MinMaxNode>(capacity)),
      pop_stack_(Stack<MinMaxNode>(capacity)) {}

Queue<MinMaxNode>::Queue(const Queue<MinMaxNode>& other) = default;

Queue<MinMaxNode>& Queue<MinMaxNode>::operator=(
    const Queue<MinMaxNode>& other) = default;

Queue<MinMaxNode>::Queue(Queue<MinMaxNode>&& other) noexcept = default;

Queue<MinMaxNode>& Queue<MinMaxNode>::operator=(
    Queue<MinMaxNode>&& other) noexcept = default;

Queue<MinMaxNode>::~Queue() = default;

// Element access
[[nodiscard]] Queue<MinMaxNode>::reference Queue<MinMaxNode>::front() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

[[nodiscard]] Queue<MinMaxNode>::const_reference Queue<MinMaxNode>::front()
    const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    return push_stack_.bottom();
  }
  return pop_stack_.top();
}

[[nodiscard]] Queue<MinMaxNode>::reference Queue<MinMaxNode>::back() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!push_stack_.empty()) {
    return push_stack_.top();
  }
  return pop_stack_.bottom();
}

[[nodiscard]] Queue<MinMaxNode>::const_reference Queue<MinMaxNode>::back()
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
[[nodiscard]] bool Queue<MinMaxNode>::empty() const noexcept {
  return size_ == 0;
}

[[nodiscard]] std::size_t Queue<MinMaxNode>::getSize() const noexcept {
  return size_;
}

// Requests
[[nodiscard]] int Queue<MinMaxNode>::getMaxDiff() const {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (push_stack_.empty()) {
    const MinMaxNode& pop_stack_top = pop_stack_.top();
    return pop_stack_top.max_value - pop_stack_top.min_value;
  }
  if (pop_stack_.empty()) {
    const MinMaxNode& push_stack_top = push_stack_.top();
    return push_stack_top.max_value - push_stack_top.min_value;
  }
  const MinMaxNode& pop_stack_top = pop_stack_.top();
  const MinMaxNode& push_stack_top = push_stack_.top();
  int max_value = push_stack_top.max_value > pop_stack_top.max_value
                      ? push_stack_top.max_value
                      : pop_stack_top.max_value;
  int min_value = push_stack_top.min_value < pop_stack_top.min_value
                      ? push_stack_top.min_value
                      : pop_stack_top.min_value;
  return max_value - min_value;
}

// Modifiers
void Queue<MinMaxNode>::pop() {
  if (size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (pop_stack_.empty()) {
    Stack<MinMaxNode>::template MoveStackContent<MinMaxNode>::get(pop_stack_,
                                                                  push_stack_);
  }
  pop_stack_.pop();
  --size_;
}

void Queue<MinMaxNode>::push(int value) {
  if (push_stack_.empty()) {
    push_stack_.push(MinMaxNode{value, value, value});
  } else {
    MinMaxNode&& node_to_push = {value, value, value};
    const MinMaxNode& top_node = push_stack_.top();
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

void Queue<MinMaxNode>::resize(size_type new_capacity) {
  pop_stack_.resize(new_capacity);
  push_stack_.resize(new_capacity);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::min_max_queue
