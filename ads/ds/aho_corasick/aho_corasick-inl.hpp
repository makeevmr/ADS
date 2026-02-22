#ifndef ADS_DS_AHO_CORASICK_AHO_CORASICK_AUTOMATA_INL_HPP_
#error \
    "Direct inclusion of this file is not allowed, include aho_corasick_automata.hpp"
// For the sake of sane code completion.
#include "aho_corasick.hpp"
#endif

#include <queue>
#include <algorithm>

namespace ads::ds::aho_corasick {

////////////////////////////////////////////////////////////////////////////////

template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
AhoCorasick<kAlphaLeft, kAlphaRight>::AhoCorasick()
    : is_built_(false),
      next_str_num_(0),
      nodes_(std::vector<Node>(1)) {}

template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
void AhoCorasick<kAlphaLeft, kAlphaRight>::addString(const std::string& s) {
  std::size_t curr_node = 0;
  for (const char& symbol : s) {
    const std::size_t symbol_ind =
        static_cast<std::size_t>(symbol - kAlphaLeft);
    if (nodes_[curr_node].next_[symbol_ind] == kUndefinedFlag) {
      is_built_ = false;
      nodes_[curr_node].next_[symbol_ind] = nodes_.size();
      nodes_.emplace_back();
    }
    curr_node = nodes_[curr_node].next_[symbol_ind];
  }
  nodes_[curr_node].is_terminal_ = true;
  nodes_[curr_node].str_num_ = next_str_num_++;
  nodes_[curr_node].str_size_ = s.size();
}

// Return pairs[index of end position of string in text, string index]
template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
[[nodiscard]] AhoCorasick<kAlphaLeft, kAlphaRight>::occurrences
AhoCorasick<kAlphaLeft, kAlphaRight>::findAllOccurrences(
    const std::string& text) {
  if (!is_built_) {
    buildAutomata();
    is_built_ = true;
  }
  std::size_t curr_node = 0;
  std::vector<OccurrenceInfo> occurences;
  const std::size_t text_size = text.size();
  for (std::size_t i = 0; i < text_size; ++i) {
    curr_node = nodes_[curr_node].next_[text[i] - kAlphaLeft];
    std::size_t traverse_back_node = curr_node;
    do {
      if (nodes_[traverse_back_node].is_terminal_) {
        occurences.push_back(OccurrenceInfo{
            .str_start_pos_ = ((i + 1) - nodes_[traverse_back_node].str_size_),
            .str_num_ = nodes_[traverse_back_node].str_num_});
      }
      traverse_back_node = nodes_[traverse_back_node].to_terminal_link_;
    } while (traverse_back_node != kNoPathFlag);
  }
  return occurences;
}

// This function must be called after all strings was added
// Aho-Corasick algorithm implementation
// Lecture: https://www.youtube.com/watch?v=V7S80KpbQpk&list=LL&index=5&t=2s
template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
void AhoCorasick<kAlphaLeft, kAlphaRight>::buildAutomata() {
  nodes_[0].suffix_link_ = kNoPathFlag;
  nodes_[0].to_terminal_link_ = kNoPathFlag;
  for (char c = kAlphaLeft; c <= kAlphaRight; ++c) {
    if (nodes_[0].next_[c - kAlphaLeft] == kUndefinedFlag) {
      nodes_[0].next_[c - kAlphaLeft] = 0;
    }
  }
  std::queue<std::size_t> nodes_queue;
  nodes_queue.push(0);
  while (!nodes_queue.empty()) {
    std::size_t parent = nodes_queue.front();
    nodes_queue.pop();
    for (char c = kAlphaLeft; c <= kAlphaRight; ++c) {
      std::size_t child = nodes_[parent].next_[c - kAlphaLeft];
      if (nodes_[child].suffix_link_ != kUndefinedFlag) {
        continue;
      }
      nodes_[child].suffix_link_ =
          (parent == 0
               ? 0
               : nodes_[nodes_[parent].suffix_link_].next_[c - kAlphaLeft]);
      const std::size_t& suff_link_node = nodes_[child].suffix_link_;
      nodes_[child].to_terminal_link_ =
          (nodes_[suff_link_node].is_terminal_
               ? suff_link_node
               : nodes_[suff_link_node].to_terminal_link_);
      for (char d = kAlphaLeft; d <= kAlphaRight; ++d) {
        if (nodes_[child].next_[d - kAlphaLeft] != kUndefinedFlag) {
          continue;
        }
        nodes_[child].next_[d - kAlphaLeft] =
            nodes_[nodes_[child].suffix_link_].next_[d - kAlphaLeft];
      }
      nodes_queue.push(child);
    }
  }
}

template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
AhoCorasick<kAlphaLeft, kAlphaRight>::Node::Node()
    : is_terminal_(false),
      str_num_(kUndefinedFlag),
      suffix_link_(kUndefinedFlag),
      to_terminal_link_(kUndefinedFlag) {
  std::fill(next_, next_ + kAlphaSize, kUndefinedFlag);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::aho_corasick
