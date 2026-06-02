#include "exc_throw_class.hpp"

#include <stdexcept>

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

TExcThrowClass::TExcThrowClass() {
  ++created_objects;
  if (created_objects % 20 == 0) {
    throw std::runtime_error("Exception in default constructor");
  }
  Data_ = new int[10];
  for (int i = 0; i < 10; ++i) {
    Data_[i] = i;
  }
}

TExcThrowClass::TExcThrowClass(const TExcThrowClass& other) {
  ++created_objects;
  if (created_objects % 20 == 0) {
    throw std::runtime_error("Exception in copy constructor");
  }
  Data_ = new int[10];
  for (int i = 0; i < 10; ++i) {
    Data_[i] = other.Data_[i];
  }
}

TExcThrowClass& TExcThrowClass::operator=(const TExcThrowClass& other) {
  if (this != &other) {
    delete[] Data_;
    Data_ = new int[10];
    for (int i = 0; i < 10; ++i) {
      Data_[i] = i;
    }
  }
  return *this;
}

TExcThrowClass::TExcThrowClass(TExcThrowClass&& other) noexcept
    : Data_(other.Data_) {
  ++created_objects;
  other.Data_ = nullptr;
}

TExcThrowClass& TExcThrowClass::operator=(TExcThrowClass&& other) noexcept {
  ++created_objects;
  if (this != &other) {
    delete[] Data_;
    Data_ = other.Data_;
    other.Data_ = nullptr;
  }
  return *this;
}

TExcThrowClass::~TExcThrowClass() {
  --created_objects;
  delete[] Data_;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
