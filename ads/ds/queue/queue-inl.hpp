#ifndef ADS_DS_QUEUE_QUEUE_INL_HPP_
#error "Direct inclusion of this file is not allowed, include queue.hpp"
// For the sake of sane code completion.
#include "queue.hpp"
#endif

#include <stdexcept>
#include <type_traits>

namespace NAds::NDs::NQueue {

////////////////////////////////////////////////////////////////////////////////

template <typename T, std::size_t N>
TQueue<T, N>::TQueue() noexcept
    : Size_(0),
      TopPointer_(0),
      BackPointer_(0) {}

template <typename T, std::size_t N>
TQueue<T, N>::TQueue(const TQueue<T, N>& other) = default;

template <typename T, std::size_t N>
TQueue<T, N>::TQueue& TQueue<T, N>::operator=(const TQueue<T, N>& other) =
    default;

template <typename T, std::size_t N>
TQueue<T, N>::TQueue(TQueue<T, N>&& other) noexcept = default;

template <typename T, std::size_t N>
TQueue<T, N>::TQueue& TQueue<T, N>::operator=(TQueue<T, N>&& other) noexcept =
    default;

template <typename T, std::size_t N>
TQueue<T, N>::~TQueue() = default;

template <typename T, std::size_t N>
[[nodiscard]] TQueue<T, N>::TReference TQueue<T, N>::front() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  return Data_[TopPointer_];
}

template <typename T, std::size_t N>
[[nodiscard]] TQueue<T, N>::TConstReference TQueue<T, N>::front() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  return Data_[TopPointer_];
}

template <typename T, std::size_t N>
[[nodiscard]] TQueue<T, N>::TReference TQueue<T, N>::back() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  std::size_t index = BackPointer_ == 0 ? N - 1 : BackPointer_ - 1;
  return Data_[index];
}

template <typename T, std::size_t N>
[[nodiscard]] TQueue<T, N>::TConstReference TQueue<T, N>::back() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  std::size_t index = BackPointer_ == 0 ? N - 1 : BackPointer_ - 1;
  return Data_[index];
}

template <typename T, std::size_t N>
[[nodiscard]] bool TQueue<T, N>::empty() const noexcept {
  return Size_ == 0;
}

template <typename T, std::size_t N>
[[nodiscard]] std::size_t TQueue<T, N>::getSize() const noexcept {
  return Size_;
}

template <typename T, std::size_t N>
void TQueue<T, N>::pop() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  ++TopPointer_ %= N;
  --Size_;
}

template <typename T, std::size_t N>
void TQueue<T, N>::push(const T& value) {
  Data_[BackPointer_] = value;
  if (Size_ == N) {
    ++TopPointer_ %= N;
    ++BackPointer_ %= N;
  } else {
    ++BackPointer_ %= N;
    ++Size_;
  }
}

template <typename T, std::size_t N>
void TQueue<T, N>::push(T&& value) {
  Data_[BackPointer_] = std::move(value);
  if (Size_ == N) {
    ++TopPointer_ %= N;
    ++BackPointer_ %= N;
  } else {
    ++BackPointer_ %= N;
    ++Size_;
  }
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
TQueue<T, 0>::TQueue(std::size_t capacity)
    : Data_(reinterpret_cast<T*>(::operator new(sizeof(T) * capacity))),
      Size_(0),
      Capacity_(capacity),
      TopPointer_(0),
      BackPointer_(0) {}

template <typename T>
TQueue<T, 0>::TQueue(const TQueue<T, 0>& other)
    : Data_(reinterpret_cast<T*>(::operator new(sizeof(T) * other.Capacity_))),
      Size_(other.Size_),
      Capacity_(other.Capacity_),
      TopPointer_(0),
      BackPointer_(other.Size_) {
  uninitializedCopy(other);
}

template <typename T>
TQueue<T, 0>::TQueue& TQueue<T, 0>::operator=(const TQueue<T, 0>& other) {
  if (this != &other) {
    T* new_data =
        reinterpret_cast<T*>(::operator new(sizeof(T) * other.Capacity_));
    std::size_t copied_objects = 0;
    std::size_t other_data_index = other.TopPointer_;
    try {
      for (; copied_objects < other.Size_; ++copied_objects) {
        new (new_data + copied_objects) T(other.Data_[other_data_index]);
        ++other_data_index %= other.Capacity_;
      }
    } catch (...) {
      if (!std::is_trivially_destructible_v<T>) {
        for (std::size_t j = 0; j < copied_objects; ++copied_objects) {
          (new_data + j)->~T();
        }
      }
      ::operator delete(new_data);
      throw;
    }
    free(Size_);
    Data_ = new_data;
    Size_ = other.Size_;
    Capacity_ = other.Capacity_;
    TopPointer_ = 0;
    BackPointer_ = Size_;
  }
  return *this;
}

template <typename T>
TQueue<T, 0>::TQueue(TQueue<T, 0>&& other) noexcept
    : Data_(nullptr),
      Size_(0),
      Capacity_(0),
      TopPointer_(0),
      BackPointer_(0) {
  swap(other);
}

template <typename T>
TQueue<T, 0>::TQueue& TQueue<T, 0>::operator=(TQueue<T, 0>&& other) noexcept {
  if (this != &other) {
    free(Size_);
    Data_ = nullptr;
    Size_ = 0;
    Capacity_ = 0;
    TopPointer_ = 0;
    BackPointer_ = 0;
    swap(other);
  }
  return *this;
}

template <typename T>
TQueue<T, 0>::~TQueue() {
  free(Size_);
}

template <typename T>
[[nodiscard]] TQueue<T, 0>::TReference TQueue<T, 0>::front() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  return Data_[TopPointer_];
}

template <typename T>
[[nodiscard]] TQueue<T, 0>::TConstReference TQueue<T, 0>::front() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  return Data_[TopPointer_];
}

template <typename T>
[[nodiscard]] TQueue<T, 0>::TReference TQueue<T, 0>::back() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  std::size_t index = BackPointer_ == 0 ? Capacity_ - 1 : BackPointer_ - 1;
  return Data_[index];
}

template <typename T>
[[nodiscard]] TQueue<T, 0>::TConstReference TQueue<T, 0>::back() const {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  std::size_t index = BackPointer_ == 0 ? Capacity_ - 1 : BackPointer_ - 1;
  return Data_[index];
}

template <typename T>
[[nodiscard]] bool TQueue<T, 0>::empty() const noexcept {
  return Size_ == 0;
}

template <typename T>
[[nodiscard]] std::size_t TQueue<T, 0>::getSize() const noexcept {
  return Size_;
}

template <typename T>
void TQueue<T, 0>::pop() {
  if (Size_ == 0) {
    throw std::length_error("Empty queue");
  }
  if (!std::is_trivially_destructible_v<T>) {
    (Data_ + TopPointer_)->~T();
  }
  ++TopPointer_ %= Capacity_;
  --Size_;
}

template <typename T>
void TQueue<T, 0>::push(const T& value) {
  if (Size_ == Capacity_) {
    resize();
  }
  new (Data_ + BackPointer_) T(value);
  ++BackPointer_ %= Capacity_;
  ++Size_;
}

template <typename T>
void TQueue<T, 0>::push(T&& value) {
  if (Size_ == Capacity_) {
    resize();
  }
  new (Data_ + BackPointer_) T(std::move(value));
  ++BackPointer_ %= Capacity_;
  ++Size_;
}

template <typename T>
void TQueue<T, 0>::swap(TQueue<T, 0>& other) noexcept {
  std::swap(Data_, other.Data_);
  std::swap(Size_, other.Size_);
  std::swap(Capacity_, other.Capacity_);
  std::swap(TopPointer_, other.TopPointer_);
  std::swap(BackPointer_, other.BackPointer_);
}

template <typename T>
void TQueue<T, 0>::free(std::size_t destructor_calls) noexcept {
  if (!std::is_trivially_destructible_v<T>) {
    std::size_t destruction_index = TopPointer_;
    std::size_t destroyed_objects = 0;
    while (destroyed_objects < destructor_calls) {
      (Data_ + destruction_index)->~T();
      ++destruction_index %= Capacity_;
      ++destroyed_objects;
    }
  }
  ::operator delete(Data_);
}

template <typename T>
void TQueue<T, 0>::uninitializedCopy(const TQueue<T, 0>& other) {
  std::size_t copied_objects = 0;
  std::size_t other_data_index = other.TopPointer_;
  try {
    for (; copied_objects < other.Size_; ++copied_objects) {
      new (Data_ + copied_objects) T(other.Data_[other_data_index]);
      ++other_data_index %= other.Capacity_;
    }
  } catch (...) {
    free(copied_objects);
    throw;
  }
}

template <typename T>
void TQueue<T, 0>::resize() {
  if (Capacity_ == 0) {
    ::operator delete(Data_);
    Capacity_ = 1;
    Data_ = reinterpret_cast<T*>(::operator new(sizeof(T) * Capacity_));
  } else {
    std::size_t new_capacity = Capacity_ * 2;
    T* new_data =
        reinterpret_cast<T*>(::operator new(sizeof(T) * new_capacity));
    std::size_t copied_objects = 0;
    std::size_t old_data_pointer = TopPointer_;
    if (std::is_move_constructible_v<T>) {
      while (copied_objects < Size_) {
        new (new_data + copied_objects) T(std::move(Data_[old_data_pointer]));
        ++old_data_pointer %= Capacity_;
        ++copied_objects;
      }
      ::operator delete(Data_);
    } else {
      try {
        while (copied_objects < Size_) {
          new (new_data + copied_objects) T(Data_[old_data_pointer]);
          ++old_data_pointer %= Capacity_;
          ++copied_objects;
        }
      } catch (...) {
        if (!std::is_trivially_destructible_v<T>) {
          for (std::size_t j = 0; j < copied_objects; ++j) {
            (new_data + j)->~T();
          }
        }
        ::operator delete(new_data);
        throw;
      }
      if (!std::is_trivially_destructible_v<T>) {
        for (std::size_t i = 0; i < Size_; ++i) {
          (Data_ + i)->~T();
        }
      }
      ::operator delete(Data_);
    }
    Data_ = new_data;
    Capacity_ = new_capacity;
    TopPointer_ = 0;
    BackPointer_ = Size_;
  }
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NQueue
