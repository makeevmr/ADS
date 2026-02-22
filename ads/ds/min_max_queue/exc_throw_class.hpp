#pragma once

#include <stdexcept>

namespace ads::ds::min_max_queue {

////////////////////////////////////////////////////////////////////////////////

class ExcThrowClass {
public:
  ExcThrowClass() {
    ++created_objects;
    if (created_objects % 20 == 0) {
      throw std::runtime_error("Exception in default constructor");
    }
    data_ = new int[10];
    for (int i = 0; i < 10; ++i) {
      data_[i] = i;
    }
  }

  ExcThrowClass(const ExcThrowClass& other) {
    ++created_objects;
    if (created_objects % 20 == 0) {
      throw std::runtime_error("Exception in copy constructor");
    }
    data_ = new int[10];
    for (int i = 0; i < 10; ++i) {
      data_[i] = other.data_[i];
    }
  }

  ExcThrowClass& operator=(const ExcThrowClass& other) {
    if (this != &other) {
      delete[] data_;
      data_ = new int[10];
      for (int i = 0; i < 10; ++i) {
        data_[i] = i;
      }
    }
    return *this;
  }

  ExcThrowClass(ExcThrowClass&& other) noexcept
      : data_(other.data_) {
    ++created_objects;
    other.data_ = nullptr;
  }

  ExcThrowClass& operator=(ExcThrowClass&& other) noexcept {
    ++created_objects;
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      other.data_ = nullptr;
    }
    return *this;
  }

  ~ExcThrowClass() {
    --created_objects;
    delete[] data_;
  }

private:
  inline static unsigned int created_objects = 0;
  int* data_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::min_max_queue
