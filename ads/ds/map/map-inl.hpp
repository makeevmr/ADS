#ifndef ADS_DS_MAP_MAP_INL_HPP_
#error "Direct inclusion of this file is not allowed, include map.hpp"
// For the sake of sane code completion.
#include "map.hpp"
#endif

namespace NAds::NDs::NMap {

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap()
    : Root_(nullptr),
      BegIter_(TIterator(nullptr)),
      EndIter_(TIterator(nullptr)),
      Size_(0),
      Comparator_(TCompare()) {}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap(const TMap& other)
    : Root_(other.Root_ == nullptr ? nullptr : new TNode(*other.Root_)),
      BegIter_(TIterator(beginNode(Root_))),
      EndIter_(nullptr),
      Size_(other.Size_),
      Comparator_(TCompare()) {}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap& TMap<TKey, T, TCompare>::operator=(
    const TMap<TKey, T, TCompare>& other) {
  if (this != &other) {
    delete Root_;
    Root_ = nullptr;
    Size_ = 0;
    BegIter_ = TIterator(nullptr);
    if (other.Root_ != nullptr) {
      Root_ = new TNode(*other.Root_);
      BegIter_ = TIterator(beginNode(Root_));
      Size_ = other.Size_;
    }
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap(TMap&& other) noexcept
    : Root_(other.Root_),
      BegIter_(other.BegIter_),
      EndIter_(nullptr),
      Size_(other.Size_),
      Comparator_(TCompare()) {
  other.Root_ = nullptr;
  other.BegIter_ = TIterator(nullptr);
  other.Size_ = 0;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TMap& TMap<TKey, T, TCompare>::operator=(
    TMap<TKey, T, TCompare>&& other) noexcept {
  if (this != &other) {
    Root_ = other.Root_;
    BegIter_ = other.BegIter_;
    Size_ = other.Size_;
    other.Root_ = nullptr;
    other.BegIter_ = TIterator(nullptr);
    other.Size_ = 0;
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::~TMap() {
  delete Root_;
  Root_ = nullptr;
  BegIter_ = TIterator(nullptr);
  Size_ = 0;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator
TMap<TKey, T, TCompare>::begin() noexcept {
  return BegIter_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::begin() const noexcept {
  return TConstIterator(BegIter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::cbegin() const noexcept {
  return TConstIterator(BegIter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator
TMap<TKey, T, TCompare>::end() noexcept {
  return EndIter_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::end() const noexcept {
  return TConstIterator(EndIter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator
TMap<TKey, T, TCompare>::cend() const noexcept {
  return TConstIterator(EndIter_);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::empty() const noexcept {
  return Size_ == 0ULL;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TSizeType
TMap<TKey, T, TCompare>::getSize() const noexcept {
  return Size_;
}

template <typename TKey, typename T, typename TCompare>
std::pair<typename TMap<TKey, T, TCompare>::TIterator, bool>
TMap<TKey, T, TCompare>::insert(TConstReference value) {
  TNode* parent = findParent(value.first);
  if ((parent != nullptr) && (parent->Value->first == value.first)) {
    return std::pair<TIterator, bool>{TIterator(parent), false};
  }
  TNode* new_node =
      new TNode({new TValueType(value), nullptr, nullptr, parent, 1});
  ++Size_;
  if (parent == nullptr) {
    Root_ = new_node;
    BegIter_ = TIterator(Root_);
  } else {
    if (Comparator_(value.first, BegIter_->first)) {
      BegIter_ = TIterator(new_node);
    }
    TNode* rebalance_node = nullptr;
    if (Comparator_(value.first, parent->Value->first)) {
      parent->Left = new_node;
      rebalance_node = parent;
    } else {
      parent->Right = new_node;
      rebalance_node = parent->Parent;
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
      rebalance_node = rebalance_node->Parent;
    }
  }
  return std::pair<TIterator, bool>{TIterator(new_node), true};
}

template <typename TKey, typename T, typename TCompare>
std::pair<typename TMap<TKey, T, TCompare>::TIterator, bool>
TMap<TKey, T, TCompare>::insert(TValueType&& value) {
  TNode* parent = findParent(value.first);
  if ((parent != nullptr) && (parent->Value->first == value.first)) {
    return std::pair<TIterator, bool>{TIterator(parent), false};
  }
  TNode* new_node = new TNode(
      {new TValueType(std::move(value)), nullptr, nullptr, parent, 1});
  ++Size_;
  if (parent == nullptr) {
    Root_ = new_node;
    BegIter_ = TIterator(Root_);
  } else {
    if (Comparator_(value.first, BegIter_->first)) {
      BegIter_ = TIterator(new_node);
    }
    TNode* rebalance_node = nullptr;
    if (Comparator_(value.first, parent->Value->first)) {
      parent->Left = new_node;
      rebalance_node = parent;
    } else {
      parent->Right = new_node;
      rebalance_node = parent->Parent;
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
      rebalance_node = rebalance_node->Parent;
    }
  }
  return std::pair<TIterator, bool>{TIterator(new_node), true};
}

template <typename TKey, typename T, typename TCompare>
void TMap<TKey, T, TCompare>::erase(
    const TMap<TKey, T, TCompare>::TKeyType& erased_key) noexcept {
  if (Root_ == nullptr) {
    return;
  }
  TNode* current_node = findNode(erased_key);
  if (current_node != nullptr) {
    TNode* rebalance_node = nullptr;
    if (current_node->Left == nullptr && current_node->Right == nullptr) {
      rebalance_node = trivialNodeErase(current_node, nullptr);
    } else if (current_node->Left != nullptr &&
               current_node->Right == nullptr) {
      rebalance_node = trivialNodeErase(current_node, current_node->Left);
    } else if (current_node->Left == nullptr &&
               current_node->Right != nullptr) {
      rebalance_node = trivialNodeErase(current_node, current_node->Right);
    } else {
      TNode* next_node = next(current_node);
      TNode* right_child = next_node->Right;
      rebalance_node = next_node->Parent;
      if (right_child != nullptr) {
        right_child->Parent = rebalance_node;
      }
      if (rebalance_node->Left == next_node) {
        rebalance_node->Left = right_child;
      } else {
        rebalance_node->Right = right_child;
      }
      std::swap(current_node->Value, next_node->Value);
      current_node->Value->second = std::move(next_node->Value->second);
      --Size_;
      next_node->Left = nullptr;
      next_node->Right = nullptr;
      next_node->Parent = nullptr;
      delete next_node;
    }
    bool is_level_changed = true;
    while ((rebalance_node != nullptr) && is_level_changed) {
      TSizeType init_level = rebalance_node->Level;
      decreaseNodeLevel(rebalance_node);
      is_level_changed = init_level != rebalance_node->Level;
      if (is_level_changed) {
        rebalance_node = skew(rebalance_node);
        if (rebalance_node->Right != nullptr) {
          skew(rebalance_node->Right);
          if (rebalance_node->Right->Right != nullptr) {
            skew(rebalance_node->Right->Right);
          }
        }
        rebalance_node = split(rebalance_node);
        if (rebalance_node->Right != nullptr) {
          split(rebalance_node->Right);
        }
      }
      rebalance_node = rebalance_node->Parent;
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
    : Value(value),
      Left(left),
      Right(right),
      Parent(parent),
      Level(level) {}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode::TNode(
    const TMap<TKey, T, TCompare>::TNode& other)
    : Value(new TValueType(*(other.Value))),
      Left(nullptr),
      Right(nullptr),
      Parent(nullptr),
      Level(other.Level) {
  if (other.Left != nullptr) {
    Left = new TNode(*other.Left);
    Left->Parent = this;
  }
  if (other.Right != nullptr) {
    Right = new TNode(*other.Right);
    Right->Parent = this;
  }
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode::~TNode() {
  delete Value;
  Value = nullptr;
  delete Left;
  delete Right;
  Left = nullptr;
  Right = nullptr;
  Parent = nullptr;
  Level = 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator::TIterator(
    TMap<TKey, T, TCompare>::TNode* ptr) noexcept
    : Ptr_(ptr) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator::TIterator(
    const TMap<TKey, T, TCompare>::TIterator& other) noexcept
    : Ptr_(other.Ptr_) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator&
TMap<TKey, T, TCompare>::TIterator::operator=(
    const TMap<TKey, T, TCompare>::TIterator& other) & noexcept {
  if (this != &other) {
    Ptr_ = other.Ptr_;
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator::~TIterator() {};

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator::TReference
TMap<TKey, T, TCompare>::TIterator::operator*() const noexcept {
  return *(Ptr_->Value);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TIterator::TPointer
TMap<TKey, T, TCompare>::TIterator::operator->() const noexcept {
  return Ptr_->Value;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TIterator&
TMap<TKey, T, TCompare>::TIterator::operator++() {
  Ptr_ = TMap::next(Ptr_);
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
  Ptr_ = TMap::prev(Ptr_);
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
  return Ptr_ == right.Ptr_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::TIterator::operator!=(
    const TIterator& right) const noexcept {
  return Ptr_ != right.Ptr_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::TConstIterator(
    TMap<TKey, T, TCompare>::TNode* ptr) noexcept
    : Ptr_(ptr) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::TConstIterator(
    const TMap<TKey, T, TCompare>::TIterator& other) noexcept
    : Ptr_(other.Ptr_) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::TConstIterator(
    const TConstIterator& other) noexcept
    : Ptr_(other.Ptr_) {};

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator&
TMap<TKey, T, TCompare>::TConstIterator::operator=(
    const TMap<TKey, T, TCompare>::TConstIterator& other) & noexcept {
  if (this != &other) {
    Ptr_ = other.Ptr_;
  }
  return *this;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator::~TConstIterator() {};

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator::TReference
TMap<TKey, T, TCompare>::TConstIterator::operator*() const noexcept {
  return *(Ptr_->Value);
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TConstIterator::TPointer
TMap<TKey, T, TCompare>::TConstIterator::operator->() const noexcept {
  return Ptr_->Value;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TConstIterator&
TMap<TKey, T, TCompare>::TConstIterator::operator++() {
  Ptr_ = TMap::next(Ptr_);
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
  Ptr_ = TMap::prev(Ptr_);
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
  return Ptr_ == right.Ptr_;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] bool TMap<TKey, T, TCompare>::TConstIterator::operator!=(
    const TMap<TKey, T, TCompare>::TConstIterator& right) const noexcept {
  return Ptr_ != right.Ptr_;
}

////////////////////////////////////////////////////////////////////////////////

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::skew(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if ((node->Left == nullptr) || (node->Level != node->Left->level)) {
    return node;
  }
  TNode* left_node = node->Left;
  node->Left = left_node->Right;
  if (left_node->Right != nullptr) {
    left_node->Right->parent = node;
  }
  left_node->Right = node;
  left_node->Parent = node->Parent;
  if (node->Parent != nullptr) {
    if (node->Parent->Left == node) {
      node->Parent->Left = left_node;
    } else {
      node->Parent->Right = left_node;
    }
  }
  node->Parent = left_node;
  if (Root_ == node) {
    Root_ = left_node;
  }
  return left_node;
}

template <typename TKey, typename T, typename TCompare>
TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::split(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node->Right == nullptr || node->Right->Right == nullptr ||
      node->Level != node->Right->Right->level) {
    return node;
  }
  TNode* right_node = node->Right;
  node->Right = right_node->Left;
  if (right_node->Left != nullptr) {
    right_node->Left->parent = node;
  }
  right_node->Left = node;
  right_node->Parent = node->Parent;
  if (node->Parent != nullptr) {
    if (node->Parent->Left == node) {
      node->Parent->Left = right_node;
    } else {
      node->Parent->Right = right_node;
    }
  }
  node->Parent = right_node;
  if (Root_ == node) {
    Root_ = right_node;
  }
  ++right_node->Level;
  return right_node;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode*
TMap<TKey, T, TCompare>::trivialNodeErase(
    TMap<TKey, T, TCompare>::TNode* node_to_erase,
    TMap<TKey, T, TCompare>::TNode* child) noexcept {
  if (child != nullptr) {
    child->Parent = node_to_erase->Parent;
  }
  TNode* parent = nullptr;
  if (Root_ != node_to_erase) {
    parent = node_to_erase->Parent;
    if (parent->Left == node_to_erase) {
      parent->Left = child;
    } else {
      parent->Right = child;
    }
  } else {
    Root_ = child;
  }
  if (BegIter_ == TIterator(node_to_erase)) {
    ++BegIter_;
  }
  --Size_;
  node_to_erase->Left = nullptr;
  node_to_erase->Right = nullptr;
  node_to_erase->Parent = nullptr;
  delete node_to_erase;
  return parent;
}

template <typename TKey, typename T, typename TCompare>
void TMap<TKey, T, TCompare>::decreaseNodeLevel(
    TMap<TKey, T, TCompare>::TNode* node) noexcept {
  TSizeType left_diff =
      node->Left != nullptr ? node->Level - node->Left->level : node->Level;
  TSizeType right_diff =
      node->Right != nullptr ? node->Level - node->Right->level : node->Level;
  if (left_diff > 1 || right_diff > 1) {
    if (node->Right != nullptr && node->Right->level == node->Level) {
      --node->Right->level;
    }
    --node->Level;
  }
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::next(
    const TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node->Right != nullptr) {
    TNode* current_node = node->Right;
    while (current_node->Left != nullptr) {
      current_node = current_node->Left;
    }
    return current_node;
  }
  TNode* parent = node->Parent;
  while (parent != nullptr && (parent->Right == node)) {
    node = parent;
    parent = node->Parent;
  }
  return parent;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::prev(
    const TMap<TKey, T, TCompare>::TNode* node) noexcept {
  if (node->Left != nullptr) {
    TNode* current_node = node->Left;
    while (current_node->Right != nullptr) {
      current_node = current_node->Right;
    }
    return current_node;
  }
  TNode* parent = node->Parent;
  while (parent != nullptr && (parent->Left == node)) {
    node = parent;
    parent = node->Parent;
  }
  return parent;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode* TMap<TKey, T, TCompare>::findNode(
    const TMap<TKey, T, TCompare>::TKeyType& search_key) const noexcept {
  TNode* node = Root_;
  while (node != nullptr) {
    const TKeyType key = node->Value->first;
    if (!(Comparator_(key, search_key)) && !(Comparator_(search_key, key))) {
      break;
    }
    if (Comparator_(search_key, key)) {
      node = node->Left;
    } else {
      node = node->Right;
    }
  }
  return node;
}

template <typename TKey, typename T, typename TCompare>
[[nodiscard]] TMap<TKey, T, TCompare>::TNode*
TMap<TKey, T, TCompare>::findParent(
    const TMap<TKey, T, TCompare>::TKeyType& search_key) const noexcept {
  TNode* parent = nullptr;
  TNode* node = Root_;
  while (node != nullptr) {
    parent = node;
    const TKeyType key = node->Value->first;
    if (!(Comparator_(key, search_key)) && !(Comparator_(search_key, key))) {
      break;
    }
    if (Comparator_(search_key, key)) {
      node = node->Left;
    } else {
      node = node->Right;
    }
  }
  if (node != nullptr || Size_ == 0) {
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
  while (node->Left != nullptr) {
    node = node->Left;
  }
  return node;
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMap
