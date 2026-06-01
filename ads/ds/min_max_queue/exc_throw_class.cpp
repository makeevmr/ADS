#include "exc_throw_class.hpp"

#include <stdexcept>

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

TExcThrowClass::TExcThrowClass() {
  ++created_objects;
  if (created_objects % 20 == 0) {
    throw std::runtime_error("Exception in default constructor");
  }
  data_ = new int[10];
  for (int i = 0; i < 10; ++i) {
    data_[i] = i;
  }
}

TExcThrowClass::TExcThrowClass(const TExcThrowClass& other) {
  ++created_objects;
  if (created_objects % 20 == 0) {
    throw std::runtime_error("Exception in copy constructor");
  }
  data_ = new int[10];
  for (int i = 0; i < 10; ++i) {
    data_[i] = other.data_[i];
  }
}

TExcThrowClass& TExcThrowClass::operator=(const TExcThrowClass& other) {
  if (this != &other) {
    delete[] data_;
    data_ = new int[10];
    for (int i = 0; i < 10; ++i) {
      data_[i] = i;
    }
  }
  return *this;
}

TExcThrowClass::TExcThrowClass(TExcThrowClass&& other) noexcept
    : data_(other.data_) {
  ++created_objects;
  other.data_ = nullptr;
}

TExcThrowClass& TExcThrowClass::operator=(TExcThrowClass&& other) noexcept {
  ++created_objects;
  if (this != &other) {
    delete[] data_;
    data_ = other.data_;
    other.data_ = nullptr;
  }
  return *this;
}

TExcThrowClass::~TExcThrowClass() {
  --created_objects;
  delete[] data_;
}
////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
