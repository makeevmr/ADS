#ifndef ADS_DS_MAP_MAP_INL_HPP_
#error "Direct inclusion of this file is not allowed, include map.hpp"
// For the sake of sane code completion.
#include "map.hpp"
#endif

namespace NAds::NDs::NMap {

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap()
    : root_(nullptr),
      b_iter_(TIterator(nullptr)),
      e_iter_(TIterator(nullptr)),
      size_(0),
      comparator_(TCompare()) {}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap(const TMap& other)
    : root_(other.root_ == nullptr ? nullptr : new TNode(*other.root_)),
      b_iter_(TIterator(beginNode(root_))),
      e_iter_(nullptr),
      size_(other.size_),
      comparator_(TCompare()) {}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap& TMap<TKey, T, TCompare>::operator=(
    const TMap<TKey, T, TCompare>& other) {
  if (this != &other) {
    delete root_;
    root_ = nullptr;
    size_ = 0;
    b_iter_ = TIterator(nullptr);
    if (other.root_ != nullptr) {
      root_ = new TNode(*other.root_);
      b_iter_ = TIterator(beginNode(root_));
      size_ = other.size_;
    }
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap(TMap&& other) noexcept
    : root_(other.root_),
      b_iter_(other.b_iter_),
      e_iter_(nullptr),
      size_(other.size_),
      comparator_(TCompare()) {
  other.root_ = nullptr;
  other.b_iter_ = TIterator(nullptr);
  other.size_ = 0;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap& TMap<TKey, T, TCompare>::operator=(
    TMap<TKey, T, TCompare>&& other) noexcept {
  if (this != &other) {
    root_ = other.root_;
    b_iter_ = other.b_iter_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.b_iter_ = TIterator(nullptr);
    other.size_ = 0;
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::~TMap() {
  delete root_;
  root_ = nullptr;
  b_iter_ = TIterator(nullptr);
  size_ = 0;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator
TMap<TKey, T, TCompare>::begin() noexcept {
  return b_iter_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::begin() const noexcept {
  return TConstIterator(b_iter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::cbegin() const noexcept {
  return TConstIterator(b_iter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator
TMap<TKey, T, TCompare>::end() noexcept {
  return e_iter_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::end() const noexcept {
  return TConstIterator(e_iter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::cend() const noexcept {
  return TConstIterator(e_iter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::empty() const noexcept {
  return size_ == 0ULL;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TSizeType
TMap<TKey, T, TCompare>::getSize() const noexcept {
  return size_;
}

template <typename TKey, typename T, typename TCompare>
std::pair<typename TMap<TKey, T, TCompare>::TIterator, bool>
TMap<TKey, T, TCompare>::insert(TConstReference value) {
  TNode* parent = findParent(value.first);
  if ((parent != nullptr) && (parent->value->first == value.first)) {
    return std::pair<TIterator, bool>{TIterator(parent), false};
  }
  TNode* new_node =
      new TNode({new TValueType(value), nullptr, nullptr, parent, 1});
  ++size_;
  if (parent == nullptr) {
    root_ = new_node;
    b_iter_ = TIterator(root_);
  } else {
    if (comparator_(value.first, b_iter_->first)) {
      b_iter_ = TIterator(new_node);
    }
    TNode* rebalance_node = nullptr;
    if (comparator_(value.first, parent->value->first)) {
      parent->left = new_node;
      rebalance_node = parent;
    } else {
      parent->right = new_node;
      rebalance_node = parent->parent;
    }
    int unchanged_nodes = 0;
    bool is_tree_changed = false;
    while ((rebalance_node != nullptr) && (unchanged_nodes < 3)) {
      is_tree_changed = rebalance_node != skew(rebalance_node);
      is_tree_changed =
          is_tree_changed || rebalance_node != split(rebalance_node);
      if (!is_tree_changed) {
        ++unchanged_nodes;
      } else {
        unchanged_nodes = 0;
      }
      rebalance_node = rebalance_node->parent;
    }
  }
  return std::pair<TIterator, bool>{TIterator(new_node), true};
}

template <typename TKey, typename T, typename TCompare>
std::pair<typename TMap<TKey, T, TCompare>::TIterator, bool>
TMap<TKey, T, TCompare>::insert(TValueType&& value) {
  TNode* parent = findParent(value.first);
  if ((parent != nullptr) && (parent->value->first == value.first)) {
    return std::pair<TIterator, bool>{TIterator(parent), false};
  }
  TNode* new_node = new TNode(
      {new TValueType(std::move(value)), nullptr, nullptr, parent, 1});
  ++size_;
  if (parent == nullptr) {
    root_ = new_node;
    b_iter_ = TIterator(root_);
  } else {
    if (comparator_(value.first, b_iter_->first)) {
      b_iter_ = TIterator(new_node);
    }
    TNode* rebalance_node = nullptr;
    if (comparator_(value.first, parent->value->first)) {
      parent->left = new_node;
      rebalance_node = parent;
    } else {
      parent->right = new_node;
      rebalance_node = parent->parent;
    }
    int unchanged_nodes = 0;
    bool is_tree_changed = false;
    while ((rebalance_node != nullptr) && (unchanged_nodes < 3)) {
      is_tree_changed = rebalance_node != skew(rebalance_node);
      is_tree_changed =
          is_tree_changed || rebalance_node != split(rebalance_node);
      if (!is_tree_changed) {
        ++unchanged_nodes;
      } else {
        unchanged_nodes = 0;
      }
      rebalance_node = rebalance_node->parent;
    }
  }
  return std::pair<TIterator, bool>{TIterator(new_node), true};
}

template <typename TKey, typename T, typename TCompare>
void TMap<TKey, T, TCompare>::erase(
    const TMap<TKey, T, TCompare>::TKeyType& erased_key) noexcept {
  if (root_ == nullptr) {
    return;
  }
  TNode* current_node = findNode(erased_key);
  if (current_node != nullptr) {
    TNode* rebalance_node = nullptr;
    if (current_node->left == nullptr && current_node->right == nullptr) {
      rebalance_node = trivialNodeErase(current_node, nullptr);
    } else if (current_node->left != nullptr &&
               current_node->right == nullptr) {
      rebalance_node = trivialNodeErase(current_node, current_node->left);
    } else if (current_node->left == nullptr &&
               current_node->right != nullptr) {
      rebalance_node = trivialNodeErase(current_node, current_node->right);
    } else {
      TNode* next_node = next(current_node);
      TNode* right_child = next_node->right;
      rebalance_node = next_node->parent;
      if (right_child != nullptr) {
        right_child->parent = rebalance_node;
      }
      if (rebalance_node->left == next_node) {
        rebalance_node->left = right_child;
      } else {
        rebalance_node->right = right_child;
      }
      std::swap(current_node->value, next_node->value);
      current_node->value->second = std::move(next_node->value->second);
      --size_;
      next_node->left = nullptr;
      next_node->right = nullptr;
      next_node->parent = nullptr;
      delete next_node;
    }
    bool is_level_changed = true;
    while ((rebalance_node != nullptr) && is_level_changed) {
      TSizeType init_level = rebalance_node->level;
      decreaseNodeLevel(rebalance_node);
      is_level_changed = init_level != rebalance_node->level;
      if (is_level_changed) {
        rebalance_node = skew(rebalance_node);
        if (rebalance_node->right != nullptr) {
          skew(rebalance_node->right);
          if (rebalance_node->right->right != nullptr) {
            skew(rebalance_node->right->right);
          }
        }
        rebalance_node = split(rebalance_node);
        if (rebalance_node->right != nullptr) {
          split(rebalance_node->right);
        }
      }
      rebalance_node = rebalance_node->parent;
    }
  }
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator TMap<TKey, T, TCompare>::find(
    const typename TMap<TKey, T, TCompare>::TKeyType& search_key) noexcept {
  return TIterator(findNode(search_key));
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator TMap<TKey, T, TCompare>::find(
    const typename TMap<TKey, T, TCompare>::TKeyType& search_key)
    const noexcept {
  return TConstIterator(findNode(search_key));
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::contains(
    const typename TMap<TKey, T, TCompare>::TKeyType& key) const noexcept {
  return findNode(key) != nullptr;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode::TNode(TMap<TKey, T, TCompare>::TPointer value,
                                      TMap<TKey, T, TCompare>::TNode* left,
                                      TMap<TKey, T, TCompare>::TNode* right,
                                      TMap<TKey, T, TCompare>::TNode* parent,
                                      TMap::TSizeType level)
    : value(value),
      left(left),
      right(right),
      parent(parent),
      level(level) {}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode::TNode(
    const TMap<TKey, T, TCompare>::TNode& other)
    : value(new TValueType(*(other.value))),
      left(nullptr),
      right(nullptr),
      parent(nullptr),
      level(other.level) {
  if (other.left != nullptr) {
    left = new TNode(*other.left);
    left->parent = this;
  }
  if (other.right != nullptr) {
    right = new TNode(*other.right);
    right->parent = this;
  }
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode::~TNode() {
  delete value;
  value = nullptr;
  delete left;
  delete right;
  left = nullptr;
  right = nullptr;
  parent = nullptr;
  level = 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator::TIterator(
    TMap<TKey, T, TCompare>::TNode* ptr) noexcept
    : ptr_(ptr) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator::TIterator(
    const TMap<TKey, T, TCompare>::TIterator& other) noexcept
    : ptr_(other.ptr_) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator&
TMap<TKey, T, TCompare>::TIterator::operator=(
    const TMap<TKey, T, TCompare>::TIterator& other) & noexcept {
  if (this != &other) {
    ptr_ = other.ptr_;
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator::~TIterator() {};

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator::TReference
TMap<TKey, T, TCompare>::TIterator::operator*() const noexcept {
  return *(ptr_->value);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator::TPointer
TMap<TKey, T, TCompare>::TIterator::operator->() const noexcept {
  return ptr_->value;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator&
TMap<TKey, T, TCompare>::TIterator::operator++() {
  ptr_ = TMap::next(ptr_);
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator
TMap<TKey, T, TCompare>::TIterator::operator++(int) {
  TIterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator&
TMap<TKey, T, TCompare>::TIterator::operator--() {
  ptr_ = TMap::prev(ptr_);
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator
TMap<TKey, T, TCompare>::TIterator::operator--(int) {
  TIterator tmp = *this;
  --(*this);
  return tmp;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::TIterator::operator==(
    const TIterator& right) const noexcept {
  return ptr_ == right.ptr_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::TIterator::operator!=(
    const TIterator& right) const noexcept {
  return ptr_ != right.ptr_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::TConstIterator(
    TMap<TKey, T, TCompare>::TNode* ptr) noexcept
    : ptr_(ptr) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::TConstIterator(
    const TMap<TKey, T, TCompare>::TIterator& other) noexcept
    : ptr_(other.ptr_) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::TConstIterator(
    const TConstIterator& other) noexcept
    : ptr_(other.ptr_) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator&
TMap<TKey, T, TCompare>::TConstIterator::operator=(
    const TMap<TKey, T, TCompare>::TConstIterator& other) & noexcept {
  if (this != &other) {
    ptr_ = other.ptr_;
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::~TConstIterator() {};

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator::TReference
TMap<TKey, T, TCompare>::TConstIterator::operator*() const noexcept {
  return *(ptr_->value);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator::TPointer
TMap<TKey, T, TCompare>::TConstIterator::operator->() const noexcept {
  return ptr_->value;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator&
TMap<TKey, T, TCompare>::TConstIterator::operator++() {
  ptr_ = TMap::next(ptr_);
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::TConstIterator::operator++(int) {
  TMap<TKey, T, TCompare>::TConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator&
TMap<TKey, T, TCompare>::TConstIterator::operator--() {
  ptr_ = TMap::prev(ptr_);
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::TConstIterator::operator--(int) {
  TMap<TKey, T, TCompare>::TConstIterator tmp = *this;
  --(*this);
  return tmp;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::TConstIterator::operator==(
    const TMap<TKey, T, TCompare>::TConstIterator& right) const noexcept {
  return ptr_ == right.ptr_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::TConstIterator::operator!=(
    const TMap<TKey, T, TCompare>::TConstIterator& right) const noexcept {
  return ptr_ != right.ptr_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::skew(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if ((node->left == nullptr) || (node->level != node->left->level)) {
    return node;
  }
  TNode* left_node = node->left;
  node->left = left_node->right;
  if (left_node->right != nullptr) {
    left_node->right->parent = node;
  }
  left_node->right = node;
  left_node->parent = node->parent;
  if (node->parent != nullptr) {
    if (node->parent->left == node) {
      node->parent->left = left_node;
    } else {
      node->parent->right = left_node;
    }
  }
  node->parent = left_node;
  if (root_ == node) {
    root_ = left_node;
  }
  return left_node;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::split(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node->right == nullptr || node->right->right == nullptr ||
      node->level != node->right->right->level) {
    return node;
  }
  TNode* right_node = node->right;
  node->right = right_node->left;
  if (right_node->left != nullptr) {
    right_node->left->parent = node;
  }
  right_node->left = node;
  right_node->parent = node->parent;
  if (node->parent != nullptr) {
    if (node->parent->left == node) {
      node->parent->left = right_node;
    } else {
      node->parent->right = right_node;
    }
  }
  node->parent = right_node;
  if (root_ == node) {
    root_ = right_node;
  }
  ++right_node->level;
  return right_node;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode*
TMap<TKey, T, TCompare>::trivialNodeErase(
    TMap<TKey, T, TCompare>::TNode* node_to_erase,
    TMap<TKey, T, TCompare>::TNode* child) noexcept {
  if (child != nullptr) {
    child->parent = node_to_erase->parent;
  }
  TNode* parent = nullptr;
  if (root_ != node_to_erase) {
    parent = node_to_erase->parent;
    if (parent->left == node_to_erase) {
      parent->left = child;
    } else {
      parent->right = child;
    }
  } else {
    root_ = child;
  }
  if (b_iter_ == TIterator(node_to_erase)) {
    ++b_iter_;
  }
  --size_;
  node_to_erase->left = nullptr;
  node_to_erase->right = nullptr;
  node_to_erase->parent = nullptr;
  delete node_to_erase;
  return parent;
}

template <typename TKey, typename T, typename TCompare>
void TMap<TKey, T, TCompare>::decreaseNodeLevel(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  TSizeType left_diff =
      node->left != nullptr ? node->level - node->left->level : node->level;
  TSizeType right_diff =
      node->right != nullptr ? node->level - node->right->level : node->level;
  if (left_diff > 1 || right_diff > 1) {
    if (node->right != nullptr && node->right->level == node->level) {
      --node->right->level;
    }
    --node->level;
  }
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::next(
    const TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node->right != nullptr) {
    TNode* current_node = node->right;
    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }
    return current_node;
  }
  TNode* parent = node->parent;
  while (parent != nullptr && (parent->right == node)) {
    node = parent;
    parent = node->parent;
  }
  return parent;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::prev(
    const TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node->left != nullptr) {
    TNode* current_node = node->left;
    while (current_node->right != nullptr) {
      current_node = current_node->right;
    }
    return current_node;
  }
  TNode* parent = node->parent;
  while (parent != nullptr && (parent->left == node)) {
    node = parent;
    parent = node->parent;
  }
  return parent;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::findNode(
    const TMap<TKey, T, TCompare>::TKeyType& search_key) const noexcept {
  TNode* node = root_;
  while (node != nullptr) {
    const TKeyType key = node->value->first;
    if (!(comparator_(key, search_key)) && !(comparator_(search_key, key))) {
      break;
    }
    if (comparator_(search_key, key)) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return node;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode*
TMap<TKey, T, TCompare>::findParent(
    const TMap<TKey, T, TCompare>::TKeyType& search_key) const noexcept {
  TNode* parent = nullptr;
  TNode* node = root_;
  while (node != nullptr) {
    parent = node;
    const TKeyType key = node->value->first;
    if (!(comparator_(key, search_key)) && !(comparator_(search_key, key))) {
      break;
    }
    if (comparator_(search_key, key)) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  if (node != nullptr || size_ == 0) {
    return node;
  }
  return parent;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode*
TMap<TKey, T, TCompare>::beginNode(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node == nullptr) {
    return nullptr;
  }
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMap
