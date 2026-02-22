#pragma once

#include <cstddef>
#include <iterator>

namespace ads::ds::map {

////////////////////////////////////////////////////////////////////////////////

// Implementation of map using AA Tree
// The comparator must satisfy strict weak ordering relation
// TODO try to implement custom Allocator
template <typename Key, typename T, typename Compare>
class Map {
private:
  struct Node;
  class Iterator;
  class ConstIterator;

public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  Map();

  Map(const Map& other);

  Map& operator=(const Map& other);

  Map(Map&& other) noexcept;

  Map& operator=(Map&& other) noexcept;

  ~Map();

  // Iterators
  [[nodiscard]] iterator begin() noexcept;

  [[nodiscard]] const_iterator begin() const noexcept;

  [[nodiscard]] const_iterator cbegin() const noexcept;

  [[nodiscard]] iterator end() noexcept;

  [[nodiscard]] const_iterator end() const noexcept;

  [[nodiscard]] const_iterator cend() const noexcept;

  // Capacity
  [[nodiscard]] bool empty() const noexcept;

  [[nodiscard]] size_type getSize() const noexcept;

  // Modifiers

  // Return a pair consisting of an iterator to the inserted element (or to
  // the element that prevented the insertion) and a bool value set to true if
  // and only if the insertion took place.
  std::pair<iterator, bool> insert(const_reference value);

  std::pair<iterator, bool> insert(value_type&& value);

  // TODO change function signature
  void erase(const key_type& erased_key) noexcept;

  // Lookup
  iterator find(const key_type& search_key) noexcept;

  const_iterator find(const key_type& search_key) const noexcept;

  [[nodiscard]] bool contains(const key_type& key) const noexcept;

private:
  struct Node {
    Node(Map::pointer value, Node* left, Node* right, Node* parent,
         Map::size_type level);

    Node(const Node& other);

    ~Node();

    Map::pointer value;
    Node* left;
    Node* right;
    Node* parent;
    Map::size_type level;
  };

  class Iterator {
  private:
    friend Map::ConstIterator;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = Map::difference_type;
    using value_type = Map::value_type;
    using reference = Map::reference;
    using pointer = Map::pointer;

    explicit Iterator(Node* ptr) noexcept;

    Iterator(const Iterator& other) noexcept;

    Iterator& operator=(const Iterator& other) & noexcept;

    ~Iterator();

    [[nodiscard]] reference operator*() const noexcept;

    [[nodiscard]] pointer operator->() const noexcept;

    Iterator& operator++();

    Iterator operator++(int);

    Iterator& operator--();

    Iterator operator--(int);

    [[nodiscard]] bool operator==(const Iterator& right) const noexcept;

    [[nodiscard]] bool operator!=(const Iterator& right) const noexcept;

  private:
    Node* ptr_;
  };

  class ConstIterator {
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = Map::difference_type;
    using value_type = Map::value_type;
    using reference = Map::const_reference;
    using pointer = Map::const_pointer;

    explicit ConstIterator(Node* ptr) noexcept;

    explicit ConstIterator(const Iterator& other) noexcept;

    ConstIterator(const ConstIterator& other) noexcept;

    ConstIterator& operator=(const ConstIterator& other) & noexcept;

    ~ConstIterator();

    [[nodiscard]] reference operator*() const noexcept;

    [[nodiscard]] pointer operator->() const noexcept;

    ConstIterator& operator++();

    ConstIterator operator++(int);

    ConstIterator& operator--();

    ConstIterator operator--(int);

    [[nodiscard]] bool operator==(const ConstIterator& right) const noexcept;

    [[nodiscard]] bool operator!=(const ConstIterator& right) const noexcept;

  private:
    Node* ptr_;
  };

  Node* skew(Node* node) noexcept;

  Node* split(Node* node) noexcept;

  // Deleting a node in case of less than two children
  // return parent of erased node
  [[nodiscard]] Node* trivialNodeErase(Node* node_to_erase,
                                       Node* child) noexcept;

  void static decreaseNodeLevel(Node* node) noexcept;

  [[nodiscard]] static Node* next(const Node* node) noexcept;

  [[nodiscard]] static Node* prev(const Node* node) noexcept;

  // return a pointer to the node containing the key, return nullptr if such a
  // key does not exist
  // TODO add noexcept condition for Compare class
  [[nodiscard]] Node* findNode(const key_type& search_key) const noexcept;

  // return a pointer to the node containing the key, otherwise return the
  // parent of the new inserted node (return nullptr if map is empty)
  [[nodiscard]] Node* findParent(const key_type& search_key) const noexcept;

  // Pointer to leftmost node
  [[nodiscard]] static Node* beginNode(Node* node) noexcept;

  Node* root_;
  iterator b_iter_;
  iterator e_iter_;
  size_type size_;
  key_compare comparator_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::map

#define ADS_DS_MAP_MAP_INL_HPP_
#include "map-inl.hpp"
#undef ADS_DS_MAP_MAP_INL_HPP_
