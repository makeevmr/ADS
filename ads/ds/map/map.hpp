#pragma once

#include <cstddef>
#include <iterator>

namespace NAds::NDs::NMap {

////////////////////////////////////////////////////////////////////////////////

// Implementation of map using AA Tree
// The comparator must satisfy strict weak ordering relation
// TODO try to implement custom Allocator
template <typename TKey, typename T, typename TCompare>
class TMap {
private:
  struct TNode;
  class TIterator;
  class TConstIterator;

public:
  using TKeyType = TKey;
  using TMappedType = T;
  using TValueType = std::pair<const TKey, T>;
  using TSizeType = std::size_t;
  using TDifferenceType = std::ptrdiff_t;
  using TKeyCompare = TCompare;
  using TReference = TValueType&;
  using TConstReference = const TValueType&;
  using TPointer = TValueType*;
  using TConstPointer = const TValueType*;

  TMap();

  TMap(const TMap& other);

  TMap& operator=(const TMap& other);

  TMap(TMap&& other) noexcept;

  TMap& operator=(TMap&& other) noexcept;

  ~TMap();

  // Iterators
  [[nodiscard]] TIterator begin() noexcept;

  [[nodiscard]] TConstIterator begin() const noexcept;

  [[nodiscard]] TConstIterator cbegin() const noexcept;

  [[nodiscard]] TIterator end() noexcept;

  [[nodiscard]] TConstIterator end() const noexcept;

  [[nodiscard]] TConstIterator cend() const noexcept;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] TSizeType getSize() const noexcept;

  // Modifiers

  // Return a pair consisting of an TIterator to the inserted element (or to
  // the element that prevented the insertion) and a bool value set to true if
  // and only if the insertion took place.
  std::pair<TIterator, bool> insert(TConstReference value);

  std::pair<TIterator, bool> insert(TValueType&& value);

  // TODO change function signature
  void erase(const TKeyType& erased_key) noexcept;

  // Lookup
  TIterator find(const TKeyType& search_key) noexcept;

  TConstIterator find(const TKeyType& search_key) const noexcept;

  [[nodiscard]] bool contains(const TKeyType& key) const noexcept;

private:
  struct TNode {
    TNode(TMap::TPointer value, TNode* left, TNode* right, TNode* parent,
          TMap::TSizeType level);

    TNode(const TNode& other);

    ~TNode();

    TMap::TPointer Value;
    TNode* Left;
    TNode* Right;
    TNode* Parent;
    TMap::TSizeType Level;
  };

  class TIterator {
  private:
    friend TMap::TConstIterator;

  public:
    using TIteratorCategory = std::bidirectional_iterator_tag;
    using TDifferenceType = TMap::TDifferenceType;
    using TValueType = TMap::TValueType;
    using TReference = TMap::TReference;
    using TPointer = TMap::TPointer;

    explicit TIterator(TNode* ptr) noexcept;

    TIterator(const TIterator& other) noexcept;

    TIterator& operator=(const TIterator& other) & noexcept;

    ~TIterator();

    [[nodiscard]] TReference operator*() const noexcept;

    [[nodiscard]] TPointer operator->() const noexcept;

    TIterator& operator++();

    TIterator operator++(int);

    TIterator& operator--();

    TIterator operator--(int);

    [[nodiscard]] bool operator==(const TIterator& right) const noexcept;

    [[nodiscard]] bool operator!=(const TIterator& right) const noexcept;

  private:
    TNode* Ptr_;
  };

  class TConstIterator {
  public:
    using TIteratorCategory = std::bidirectional_iterator_tag;
    using TDifferenceType = TMap::TDifferenceType;
    using TValueType = TMap::TValueType;
    using TReference = TMap::TConstReference;
    using TPointer = TMap::TConstPointer;

    explicit TConstIterator(TNode* ptr) noexcept;

    explicit TConstIterator(const TIterator& other) noexcept;

    TConstIterator(const TConstIterator& other) noexcept;

    TConstIterator& operator=(const TConstIterator& other) & noexcept;

    ~TConstIterator();

    [[nodiscard]] TReference operator*() const noexcept;

    [[nodiscard]] TPointer operator->() const noexcept;

    TConstIterator& operator++();

    TConstIterator operator++(int);

    TConstIterator& operator--();

    TConstIterator operator--(int);

    [[nodiscard]] bool operator==(const TConstIterator& right) const noexcept;

    [[nodiscard]] bool operator!=(const TConstIterator& right) const noexcept;

  private:
    TNode* Ptr_;
  };

  TNode* skew(TNode* node) noexcept;

  TNode* split(TNode* node) noexcept;

  // Deleting a node in case of less than two children
  // return parent of erased node
  [[nodiscard]] TNode* trivialNodeErase(TNode* node_to_erase,
                                        TNode* child) noexcept;

  void static decreaseNodeLevel(TNode* node) noexcept;

  [[nodiscard]] static TNode* next(const TNode* node) noexcept;

  [[nodiscard]] static TNode* prev(const TNode* node) noexcept;

  // return a pointer to the node containing the key, return nullptr if such a
  // key does not exist
  // TODO add noexcept condition for Compare class
  [[nodiscard]] TNode* findNode(const TKeyType& search_key) const noexcept;

  // return a pointer to the node containing the key, otherwise return the
  // parent of the new inserted node (return nullptr if map is empty)
  [[nodiscard]] TNode* findParent(const TKeyType& search_key) const noexcept;

  // Pointer to leftmost node
  [[nodiscard]] static TNode* beginNode(TNode* node) noexcept;

  TNode* Root_;
  TIterator BegIter_;
  TIterator EndIter_;
  TSizeType Size_;
  TKeyCompare Comparator_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NMap

#define ADS_DS_MAP_MAP_INL_HPP_
#include "map-inl.hpp"
#undef ADS_DS_MAP_MAP_INL_HPP_
