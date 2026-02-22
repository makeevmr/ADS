#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <limits>

namespace ads::ds::aho_corasick {

////////////////////////////////////////////////////////////////////////////////

// TODO: add tests
template <char kAlphaLeft, char kAlphaRight>
requires(kAlphaRight >= kAlphaLeft)
class AhoCorasick {
private:
  struct OccurrenceInfo;

public:
  using occurrences = std::vector<OccurrenceInfo>;

  AhoCorasick();

  void addString(const std::string& s);

  // Return pairs[index of end position of string in text, string index]
  [[nodiscard]] occurrences findAllOccurrences(const std::string& text);

private:
  static constexpr std::int64_t kAlphaSize =
      static_cast<std::int64_t>(kAlphaRight - kAlphaLeft) + 1;
  static constexpr std::size_t kUndefinedFlag =
      std::numeric_limits<std::size_t>::max();
  static constexpr std::size_t kNoPathFlag = kUndefinedFlag - 1;

  // This function must be called after all strings was added
  // Aho-Corasick algorithm implementation
  // Lecture: https://www.youtube.com/watch?v=V7S80KpbQpk&list=LL&index=5&t=2s
  void buildAutomata();

  struct Node {
    Node();

    bool is_terminal_;
    std::size_t str_num_;
    std::size_t str_size_;
    std::size_t suffix_link_;
    std::size_t to_terminal_link_;
    std::size_t next_[kAlphaSize];
  };

  struct OccurrenceInfo {
    std::size_t str_start_pos_;
    std::size_t str_num_;
  };

  bool is_built_;
  std::size_t next_str_num_;
  std::vector<Node> nodes_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace ads::ds::aho_corasick

#define ADS_DS_AHO_CORASICK_AHO_CORASICK_AUTOMATA_INL_HPP_
#include "aho_corasick-inl.hpp"
#undef ADS_DS_AHO_CORASICK_AHO_CORASICK_AUTOMATA_INL_HPP_
