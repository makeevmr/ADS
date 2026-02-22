#ifndef ADS_DS_HEAP_COMPARE_INL_HPP_
#error \
    "Direct inclusion of this file is not allowed, include aho_corasick_automata.hpp"
// For the sake of sane code completion.
#include "compare.hpp"
#endif

namespace ads::ds::heap {

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool MoreCompare<T>::operator()(const T& left, const T& right) {
  return left > right;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::heap
