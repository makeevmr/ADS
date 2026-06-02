#ifndef ADS_DS_SEGMENT_TREE_SEGMENT_TREE_INL_HPP_
#error "Direct inclusion of this file is not allowed, include segment_tree.hpp"
// For the sake of sane code completion.
#include "segment_tree.hpp"
#endif

#include <stdexcept>

namespace NAds::NDs::NSegmentTree {

////////////////////////////////////////////////////////////////////////////////

template <typename T, typename TFunctor, T NeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
TSegmentTree<T, TFunctor, NeutralElement>::TSegmentTree(
    const std::vector<T>& vec)
    : BinOperation_(),
      VecSize_(vec.size()),
      SegmentTree_(4 * vec.size()) {
  if (vec.empty()) {
    throw std::runtime_error("Base vector must be non empty");
  }
  build(vec, 0ULL, 0ULL, vec.size() - 1);
}

template <typename T, typename TFunctor, T NeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
[[nodiscard]] T TSegmentTree<T, TFunctor, NeutralElement>::segmentQuery(
    const std::size_t& left, const std::size_t& right) const {
  if (left > right) {
    throw std::range_error(
        "Left index of the query must be not greater than right one");
  }
  if (right >= VecSize_) {
    throw std::range_error("The segment exceeds the size of the vector");
  }
  return subtreeSegmentQuery(0ULL, 0ULL, VecSize_ - 1, left, right);
}

template <typename T, typename TFunctor, T NeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
void TSegmentTree<T, TFunctor, NeutralElement>::indexUpdate(
    const std::size_t& vec_ind, const T& new_vec_value) {
  if (vec_ind >= VecSize_) {
    throw std::range_error("Index exceeds the size of the vector");
  }
  subtreeIndexUpdate(0ULL, 0ULL, VecSize_ - 1, vec_ind, new_vec_value);
}

template <typename T, typename TFunctor, T NeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
void TSegmentTree<T, TFunctor, NeutralElement>::build(
    const std::vector<T>& base_array, const std::size_t& tree_ind,
    const std::size_t& segment_left, const std::size_t& segment_right) {
  if (segment_left == segment_right) {
    SegmentTree_[tree_ind] = base_array[segment_left];
  } else {
    const std::size_t segment_middle = (segment_left + segment_right) / 2;
    const std::size_t tree_left_ind = tree_ind * 2 + 1;
    const std::size_t tree_right_ind = tree_ind * 2 + 2;
    build(base_array, tree_left_ind, segment_left, segment_middle);
    build(base_array, tree_right_ind, segment_middle + 1, segment_right);
    SegmentTree_[tree_ind] = BinOperation_(SegmentTree_[tree_left_ind],
                                           SegmentTree_[tree_right_ind]);
  }
}

template <typename T, typename TFunctor, T NeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
[[nodiscard]] T TSegmentTree<T, TFunctor, NeutralElement>::subtreeSegmentQuery(
    const std::size_t& tree_ind, const std::size_t& segment_left,
    const std::size_t& segment_right, const std::size_t& query_left,
    const std::size_t& query_right) const {
  if (query_left > query_right) {
    return NeutralElement;
  }
  if ((segment_left == query_left) && (segment_right == query_right)) {
    return SegmentTree_[tree_ind];
  }
  const std::size_t segment_middle = (segment_left + segment_right) / 2;
  const std::size_t tree_left_ind = tree_ind * 2 + 1;
  const std::size_t tree_right_ind = tree_ind * 2 + 2;
  return BinOperation_(
      subtreeSegmentQuery(tree_left_ind, segment_left, segment_middle,
                          query_left, std::min(query_right, segment_middle)),
      subtreeSegmentQuery(tree_right_ind, segment_middle + 1, segment_right,
                          std::max(query_left, segment_middle + 1),
                          query_right));
}

template <typename T, typename TFunctor, T NeutralElement>
requires CBinaryOperator<TFunctor, T> && std::is_copy_assignable_v<T>
void TSegmentTree<T, TFunctor, NeutralElement>::subtreeIndexUpdate(
    const std::size_t& tree_ind, const std::size_t& segment_left,
    const std::size_t& segment_right, const std::size_t& vec_ind,
    const T& new_vec_value) {
  if (segment_left == segment_right) {
    SegmentTree_[tree_ind] = new_vec_value;
    return;
  }
  const std::size_t segment_middle = (segment_left + segment_right) / 2;
  const std::size_t tree_left_ind = tree_ind * 2 + 1;
  const std::size_t tree_right_ind = tree_ind * 2 + 2;
  if (vec_ind <= segment_middle) {
    subtreeIndexUpdate(tree_left_ind, segment_left, segment_middle, vec_ind,
                       new_vec_value);
  } else {
    subtreeIndexUpdate(tree_right_ind, segment_middle + 1, segment_right,
                       vec_ind, new_vec_value);
  }
  SegmentTree_[tree_ind] =
      BinOperation_(SegmentTree_[tree_left_ind], SegmentTree_[tree_right_ind]);
}
////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NSegmentTree
