#pragma once

#include <stdexcept>
#include <vector>
#include <type_traits>

namespace ads::ds::segment_tree {

////////////////////////////////////////////////////////////////////////////////

template <typename Functor, typename ArgType>
concept BinaryOperator =
    requires(Functor func_obj, ArgType arg1, ArgType arg2) {
      { func_obj(arg1, arg2) } -> std::same_as<ArgType>;
    };

////////////////////////////////////////////////////////////////////////////////

// TODO add methods:
// 1. To assign new_value to all elements in segment_tree_[left, ..., right]
template <typename T, typename Functor, T kNeutralElement>
requires BinaryOperator<Functor, T> && std::is_copy_assignable_v<T>
class SegmentTree {
public:
  explicit SegmentTree(const std::vector<T>& vec);

  [[nodiscard]] T segmentQuery(const std::size_t& left,
                               const std::size_t& right) const;

  void indexUpdate(const std::size_t& vec_ind, const T& new_vec_value);

private:
  void build(const std::vector<T>& base_array, const std::size_t& tree_ind,
             const std::size_t& segment_left, const std::size_t& segment_right);

  [[nodiscard]] T subtreeSegmentQuery(const std::size_t& tree_ind,
                                      const std::size_t& segment_left,
                                      const std::size_t& segment_right,
                                      const std::size_t& query_left,
                                      const std::size_t& query_right) const;

  void subtreeIndexUpdate(const std::size_t& tree_ind,
                          const std::size_t& segment_left,
                          const std::size_t& segment_right,
                          const std::size_t& vec_ind, const T& new_vec_value);

  Functor bin_operation_;
  std::size_t vec_size_;
  std::vector<T> segment_tree_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::segment_tree

#define ADS_DS_SEGMENT_TREE_SEGMENT_TREE_INL_HPP_
#include "segment_tree-inl.hpp"
#undef ADS_DS_SEGMENT_TREE_SEGMENT_TREE_INL_HPP_
