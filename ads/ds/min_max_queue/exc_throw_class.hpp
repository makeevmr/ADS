#pragma once

namespace NAds::NDs::NMinMaxQueue {

////////////////////////////////////////////////////////////////////////////////

class TExcThrowClass {
public:
  TExcThrowClass();

  TExcThrowClass(const TExcThrowClass& other);

  TExcThrowClass& operator=(const TExcThrowClass& other);

  TExcThrowClass(TExcThrowClass&& other) noexcept;

  TExcThrowClass& operator=(TExcThrowClass&& other) noexcept;

  ~TExcThrowClass();

private:
  inline static unsigned int created_objects = 0;
  int* Data_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMinMaxQueue
