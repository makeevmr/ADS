#pragma once

namespace ads::ds::heap {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class MoreCompare {
public:
  bool operator()(const T& left, const T& right);
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::heap

#define ADS_DS_HEAP_COMPARE_INL_HPP_
#include "compare-inl.hpp"
#undef ADS_DS_HEAP_COMPARE_INL_HPP_
