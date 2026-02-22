#ifndef ADS_DS_MAP_MAP_INL_HPP_
#error "Direct inclusion of this file is not allowed, include map.hpp"
// For the sake of sane code completion.
#include "map.hpp"
#endif

namespace ads::ds::map {

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Map()
    : root_(nullptr),
      b_iter_(Iterator(nullptr)),
      e_iter_(Iterator(nullptr)),
      size_(0),
      comparator_(Compare()) {}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Map(const Map& other)
    : root_(other.root_ == nullptr ? nullptr : new Node(*other.root_)),
      b_iter_(Iterator(beginNode(root_))),
      e_iter_(nullptr),
      size_(other.size_),
      comparator_(Compare()) {}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Map& Map<Key, T, Compare>::operator=(
    const Map<Key, T, Compare>& other) {
  if (this != &other) {
    delete root_;
    root_ = nullptr;
    size_ = 0;
    b_iter_ = Iterator(nullptr);
    if (other.root_ != nullptr) {
      root_ = new Node(*other.root_);
      b_iter_ = Iterator(beginNode(root_));
      size_ = other.size_;
    }
  }
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Map(Map&& other) noexcept
    : root_(other.root_),
      b_iter_(other.b_iter_),
      e_iter_(nullptr),
      size_(other.size_),
      comparator_(Compare()) {
  other.root_ = nullptr;
  other.b_iter_ = Iterator(nullptr);
  other.size_ = 0;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Map& Map<Key, T, Compare>::operator=(
    Map<Key, T, Compare>&& other) noexcept {
  if (this != &other) {
    root_ = other.root_;
    b_iter_ = other.b_iter_;
    size_ = other.size_;
    other.root_ = nullptr;
    other.b_iter_ = Iterator(nullptr);
    other.size_ = 0;
  }
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::~Map() {
  delete root_;
  root_ = nullptr;
  b_iter_ = Iterator(nullptr);
  size_ = 0;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::iterator
Map<Key, T, Compare>::begin() noexcept {
  return b_iter_;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::const_iterator Map<Key, T, Compare>::begin()
    const noexcept {
  return ConstIterator(b_iter_);
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::const_iterator
Map<Key, T, Compare>::cbegin() const noexcept {
  return ConstIterator(b_iter_);
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::iterator
Map<Key, T, Compare>::end() noexcept {
  return e_iter_;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::const_iterator Map<Key, T, Compare>::end()
    const noexcept {
  return ConstIterator(e_iter_);
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::const_iterator Map<Key, T, Compare>::cend()
    const noexcept {
  return ConstIterator(e_iter_);
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] bool Map<Key, T, Compare>::empty() const noexcept {
  return size_ == 0ULL;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::size_type Map<Key, T, Compare>::getSize()
    const noexcept {
  return size_;
}

template <typename Key, typename T, typename Compare>
std::pair<typename Map<Key, T, Compare>::iterator, bool>
Map<Key, T, Compare>::insert(const_reference value) {
  Node* parent = findParent(value.first);
  if ((parent != nullptr) && (parent->value->first == value.first)) {
    return std::pair<iterator, bool>{Iterator(parent), false};
  }
  Node* new_node =
      new Node({new value_type(value), nullptr, nullptr, parent, 1});
  ++size_;
  if (parent == nullptr) {
    root_ = new_node;
    b_iter_ = Iterator(root_);
  } else {
    if (comparator_(value.first, b_iter_->first)) {
      b_iter_ = Iterator(new_node);
    }
    Node* rebalance_node = nullptr;
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
  return std::pair<iterator, bool>{Iterator(new_node), true};
}

template <typename Key, typename T, typename Compare>
std::pair<typename Map<Key, T, Compare>::iterator, bool>
Map<Key, T, Compare>::insert(value_type&& value) {
  Node* parent = findParent(value.first);
  if ((parent != nullptr) && (parent->value->first == value.first)) {
    return std::pair<iterator, bool>{Iterator(parent), false};
  }
  Node* new_node =
      new Node({new value_type(std::move(value)), nullptr, nullptr, parent, 1});
  ++size_;
  if (parent == nullptr) {
    root_ = new_node;
    b_iter_ = Iterator(root_);
  } else {
    if (comparator_(value.first, b_iter_->first)) {
      b_iter_ = Iterator(new_node);
    }
    Node* rebalance_node = nullptr;
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
  return std::pair<iterator, bool>{Iterator(new_node), true};
}

template <typename Key, typename T, typename Compare>
void Map<Key, T, Compare>::erase(
    const Map<Key, T, Compare>::key_type& erased_key) noexcept {
  if (root_ == nullptr) {
    return;
  }
  Node* current_node = findNode(erased_key);
  if (current_node != nullptr) {
    Node* rebalance_node = nullptr;
    if (current_node->left == nullptr && current_node->right == nullptr) {
      rebalance_node = trivialNodeErase(current_node, nullptr);
    } else if (current_node->left != nullptr &&
               current_node->right == nullptr) {
      rebalance_node = trivialNodeErase(current_node, current_node->left);
    } else if (current_node->left == nullptr &&
               current_node->right != nullptr) {
      rebalance_node = trivialNodeErase(current_node, current_node->right);
    } else {
      Node* next_node = next(current_node);
      Node* right_child = next_node->right;
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
      size_type init_level = rebalance_node->level;
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

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::iterator Map<Key, T, Compare>::find(
    const typename Map<Key, T, Compare>::key_type& search_key) noexcept {
  return Iterator(findNode(search_key));
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::const_iterator Map<Key, T, Compare>::find(
    const typename Map<Key, T, Compare>::key_type& search_key) const noexcept {
  return ConstIterator(findNode(search_key));
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] bool Map<Key, T, Compare>::contains(
    const typename Map<Key, T, Compare>::key_type& key) const noexcept {
  return findNode(key) != nullptr;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Node::Node(Map<Key, T, Compare>::pointer value,
                                 Map<Key, T, Compare>::Node* left,
                                 Map<Key, T, Compare>::Node* right,
                                 Map<Key, T, Compare>::Node* parent,
                                 Map::size_type level)
    : value(value),
      left(left),
      right(right),
      parent(parent),
      level(level) {}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Node::Node(const Map<Key, T, Compare>::Node& other)
    : value(new value_type(*(other.value))),
      left(nullptr),
      right(nullptr),
      parent(nullptr),
      level(other.level) {
  if (other.left != nullptr) {
    left = new Node(*other.left);
    left->parent = this;
  }
  if (other.right != nullptr) {
    right = new Node(*other.right);
    right->parent = this;
  }
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Node::~Node() {
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

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator::Iterator(
    Map<Key, T, Compare>::Node* ptr) noexcept
    : ptr_(ptr){};

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator::Iterator(
    const Map<Key, T, Compare>::Iterator& other) noexcept
    : ptr_(other.ptr_){};

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator& Map<Key, T, Compare>::Iterator::operator=(
    const Map<Key, T, Compare>::Iterator& other) & noexcept {
  if (this != &other) {
    ptr_ = other.ptr_;
  }
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator::~Iterator(){};

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Iterator::reference
Map<Key, T, Compare>::Iterator::operator*() const noexcept {
  return *(ptr_->value);
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Iterator::pointer
Map<Key, T, Compare>::Iterator::operator->() const noexcept {
  return ptr_->value;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator& Map<Key, T, Compare>::Iterator::operator++() {
  ptr_ = Map::next(ptr_);
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator Map<Key, T, Compare>::Iterator::operator++(int) {
  Iterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator& Map<Key, T, Compare>::Iterator::operator--() {
  ptr_ = Map::prev(ptr_);
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Iterator Map<Key, T, Compare>::Iterator::operator--(int) {
  Iterator tmp = *this;
  --(*this);
  return tmp;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] bool Map<Key, T, Compare>::Iterator::operator==(
    const Iterator& right) const noexcept {
  return ptr_ == right.ptr_;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] bool Map<Key, T, Compare>::Iterator::operator!=(
    const Iterator& right) const noexcept {
  return ptr_ != right.ptr_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator::ConstIterator(
    Map<Key, T, Compare>::Node* ptr) noexcept
    : ptr_(ptr){};

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator::ConstIterator(
    const Map<Key, T, Compare>::Iterator& other) noexcept
    : ptr_(other.ptr_){};

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator::ConstIterator(
    const ConstIterator& other) noexcept
    : ptr_(other.ptr_){};

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator&
Map<Key, T, Compare>::ConstIterator::operator=(
    const Map<Key, T, Compare>::ConstIterator& other) & noexcept {
  if (this != &other) {
    ptr_ = other.ptr_;
  }
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator::~ConstIterator(){};

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::ConstIterator::reference
Map<Key, T, Compare>::ConstIterator::operator*() const noexcept {
  return *(ptr_->value);
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::ConstIterator::pointer
Map<Key, T, Compare>::ConstIterator::operator->() const noexcept {
  return ptr_->value;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator&
Map<Key, T, Compare>::ConstIterator::operator++() {
  ptr_ = Map::next(ptr_);
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator
Map<Key, T, Compare>::ConstIterator::operator++(int) {
  Map<Key, T, Compare>::ConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator&
Map<Key, T, Compare>::ConstIterator::operator--() {
  ptr_ = Map::prev(ptr_);
  return *this;
}

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::ConstIterator
Map<Key, T, Compare>::ConstIterator::operator--(int) {
  Map<Key, T, Compare>::ConstIterator tmp = *this;
  --(*this);
  return tmp;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] bool Map<Key, T, Compare>::ConstIterator::operator==(
    const Map<Key, T, Compare>::ConstIterator& right) const noexcept {
  return ptr_ == right.ptr_;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] bool Map<Key, T, Compare>::ConstIterator::operator!=(
    const Map<Key, T, Compare>::ConstIterator& right) const noexcept {
  return ptr_ != right.ptr_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Node* Map<Key, T, Compare>::skew(
    Map<Key, T, Compare>::Node* node) noexcept {
  if ((node->left == nullptr) || (node->level != node->left->level)) {
    return node;
  }
  Node* left_node = node->left;
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

template <typename Key, typename T, typename Compare>
Map<Key, T, Compare>::Node* Map<Key, T, Compare>::split(
    Map<Key, T, Compare>::Node* node) noexcept {
  if (node->right == nullptr || node->right->right == nullptr ||
      node->level != node->right->right->level) {
    return node;
  }
  Node* right_node = node->right;
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

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Node*
Map<Key, T, Compare>::trivialNodeErase(
    Map<Key, T, Compare>::Node* node_to_erase,
    Map<Key, T, Compare>::Node* child) noexcept {
  if (child != nullptr) {
    child->parent = node_to_erase->parent;
  }
  Node* parent = nullptr;
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
  if (b_iter_ == Iterator(node_to_erase)) {
    ++b_iter_;
  }
  --size_;
  node_to_erase->left = nullptr;
  node_to_erase->right = nullptr;
  node_to_erase->parent = nullptr;
  delete node_to_erase;
  return parent;
}

template <typename Key, typename T, typename Compare>
void Map<Key, T, Compare>::decreaseNodeLevel(
    Map<Key, T, Compare>::Node* node) noexcept {
  size_type left_diff =
      node->left != nullptr ? node->level - node->left->level : node->level;
  size_type right_diff =
      node->right != nullptr ? node->level - node->right->level : node->level;
  if (left_diff > 1 || right_diff > 1) {
    if (node->right != nullptr && node->right->level == node->level) {
      --node->right->level;
    }
    --node->level;
  }
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Node* Map<Key, T, Compare>::next(
    const Map<Key, T, Compare>::Node* node) noexcept {
  if (node->right != nullptr) {
    Node* current_node = node->right;
    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }
    return current_node;
  }
  Node* parent = node->parent;
  while (parent != nullptr && (parent->right == node)) {
    node = parent;
    parent = node->parent;
  }
  return parent;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Node* Map<Key, T, Compare>::prev(
    const Map<Key, T, Compare>::Node* node) noexcept {
  if (node->left != nullptr) {
    Node* current_node = node->left;
    while (current_node->right != nullptr) {
      current_node = current_node->right;
    }
    return current_node;
  }
  Node* parent = node->parent;
  while (parent != nullptr && (parent->left == node)) {
    node = parent;
    parent = node->parent;
  }
  return parent;
}

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Node* Map<Key, T, Compare>::findNode(
    const Map<Key, T, Compare>::key_type& search_key) const noexcept {
  Node* node = root_;
  while (node != nullptr) {
    const key_type key = node->value->first;
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

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Node* Map<Key, T, Compare>::findParent(
    const Map<Key, T, Compare>::key_type& search_key) const noexcept {
  Node* parent = nullptr;
  Node* node = root_;
  while (node != nullptr) {
    parent = node;
    const key_type key = node->value->first;
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

template <typename Key, typename T, typename Compare>
[[nodiscard]] Map<Key, T, Compare>::Node* Map<Key, T, Compare>::beginNode(
    Map<Key, T, Compare>::Node* node) noexcept {
  if (node == nullptr) {
    return nullptr;
  }
  while (node->left != nullptr) {
    node = node->left;
  }
  return node;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::map
