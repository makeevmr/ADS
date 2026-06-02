#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <limits>

namespace NAds::NDs::NAhoCorasick {

////////////////////////////////////////////////////////////////////////////////

// TODO: add tests
template <char AlphaLeft, char AlphaRight>
requires(AlphaRight >= AlphaLeft)
class TAhoCorasick {
private:
  struct TOccurrenceInfo;

public:
  using TOccurrences = std::vector<TOccurrenceInfo>;

  TAhoCorasick();

  void addString(const std::string& s);

  // Return pairs[index of end position of string in text, string index]
  [[nodiscard]] TOccurrences findAllOccurrences(const std::string& text);

private:
  static constexpr std::int64_t AlphaSize =
      static_cast<std::int64_t>(AlphaRight - AlphaLeft) + 1;
  static constexpr std::size_t UndefinedFlag =
      std::numeric_limits<std::size_t>::max();
  static constexpr std::size_t NoPathFlag = UndefinedFlag - 1;

  // This function must be called after all strings was added
  // Aho-Corasick algorithm implementation
  // Lecture: https://www.youtube.com/watch?v=V7S80KpbQpk&list=LL&index=5&t=2s
  void buildAutomata();

  struct TNode {
    TNode();

    bool IsTerminal;
    std::size_t StrNum;
    std::size_t StrSize;
    std::size_t SuffixLink;
    std::size_t ToTerminalLink;
    std::size_t Next[AlphaSize];
  };

  struct TOccurrenceInfo {
    std::size_t StrStartPos;
    std::size_t StrNum;
  };

  bool IsBuilt_;
  std::size_t NextStrNum_;
  std::vector<TNode> Nodes_;
};

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NAhoCorasick

#define ADS_DS_AHO_CORASICK_AHO_CORASICK_AUTOMATA_INL_HPP_
#include "aho_corasick-inl.hpp"
#undef ADS_DS_AHO_CORASICK_AHO_CORASICK_AUTOMATA_INL_HPP_
