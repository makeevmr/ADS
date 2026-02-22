#pragma once

namespace ads::ds::min_max_queue {

////////////////////////////////////////////////////////////////////////////////

class ExcThrowClass {
public:
  ExcThrowClass();

  ExcThrowClass(const ExcThrowClass& other);

  ExcThrowClass& operator=(const ExcThrowClass& other);

  ExcThrowClass(ExcThrowClass&& other) noexcept;

  ExcThrowClass& operator=(ExcThrowClass&& other) noexcept;

  ~ExcThrowClass();

private:
  inline static unsigned int created_objects = 0;
  int* data_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::min_max_queue
