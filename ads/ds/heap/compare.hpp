#pragma once

namespace NAds::NDs::NHeap {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class TMoreCompare {
public:
  bool operator()(const T& left, const T& right);
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NHeap

#define ADS_DS_HEAP_COMPARE_INL_HPP_
#include "compare-inl.hpp"
#undef ADS_DS_HEAP_COMPARE_INL_HPP_
