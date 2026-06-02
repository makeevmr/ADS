#pragma once

#include <vector>
#include <type_traits>

namespace NAds::NDs::NSegmentTree {

////////////////////////////////////////////////////////////////////////////////

template <typename TFunctor, typename TArgType>
concept CBinaryOperator =
    requires(TFunctor func_obj, TArgType arg1, TArgType arg2) {
      { func_obj(arg1, arg2) } -> std::same_as<TArgType>;
    };

////////////////////////////////////////////////////////////////////////////////

// TODO add methods:
// 1. To assign new_value to all elements in segment_tree_[left, ..., right]
template <typename T, typename TFunctor, T kNeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
class TSegmentTree {
public:
  explicit TSegmentTree(const std::vector<T>& vec);

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

  TFunctor bin_operation_;
  std::size_t vec_size_;
  std::vector<T> segment_tree_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NSegmentTree

#define ADS_DS_SEGMENT_TREE_SEGMENT_TREE_INL_HPP_
#include "segment_tree-inl.hpp"
#undef ADS_DS_SEGMENT_TREE_SEGMENT_TREE_INL_HPP_
