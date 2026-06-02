#ifndef ADS_DS_AHO_CORASICK_AHO_CORASICK_AUTOMATA_INL_HPP_
#error "Direct inclusion of this file is not allowed, include aho_corasick.hpp"
// For the sake of sane code completion.
#include "aho_corasick.hpp"
#endif

#include <queue>
#include <algorithm>

namespace NAds::NDs::NAhoCorasick {

////////////////////////////////////////////////////////////////////////////////

template <char AlphaLeft, char AlphaRight>
requires(AlphaRight >= AlphaLeft)
TAhoCorasick<AlphaLeft, AlphaRight>::TAhoCorasick()
    : IsBuilt_(false),
      NextStrNum_(0),
      Nodes_(std::vector<TNode>(1)) {}

template <char AlphaLeft, char AlphaRight>
requires(AlphaRight >= AlphaLeft)
void TAhoCorasick<AlphaLeft, AlphaRight>::addString(const std::string& s) {
  std::size_t curr_node = 0;
  for (const char& symbol : s) {
    const std::size_t symbol_ind = static_cast<std::size_t>(symbol - AlphaLeft);
    if (Nodes_[curr_node].Next[symbol_ind] == UndefinedFlag) {
      IsBuilt_ = false;
      Nodes_[curr_node].Next[symbol_ind] = Nodes_.size();
      Nodes_.emplace_back();
    }
    curr_node = Nodes_[curr_node].Next[symbol_ind];
  }
  Nodes_[curr_node].IsTerminal = true;
  Nodes_[curr_node].StrNum = NextStrNum_++;
  Nodes_[curr_node].StrSize = s.size();
}

// Return pairs[index of end position of string in text, string index]
template <char AlphaLeft, char AlphaRight>
requires(AlphaRight >= AlphaLeft)
[[nodiscard]] TAhoCorasick<AlphaLeft, AlphaRight>::TOccurrences
TAhoCorasick<AlphaLeft, AlphaRight>::findAllOccurrences(
    const std::string& text) {
  if (!IsBuilt_) {
    buildAutomata();
    IsBuilt_ = true;
  }
  std::size_t curr_node = 0;
  std::vector<TOccurrenceInfo> occurences;
  const std::size_t text_size = text.size();
  for (std::size_t i = 0; i < text_size; ++i) {
    curr_node = Nodes_[curr_node].Next[text[i] - AlphaLeft];
    std::size_t traverse_back_node = curr_node;
    do {
      if (Nodes_[traverse_back_node].IsTerminal) {
        occurences.push_back(TOccurrenceInfo{
            .StrStartPos = ((i + 1) - Nodes_[traverse_back_node].StrSize),
            .StrNum = Nodes_[traverse_back_node].StrNum});
      }
      traverse_back_node = Nodes_[traverse_back_node].ToTerminalLink;
    } while (traverse_back_node != NoPathFlag);
  }
  return occurences;
}

// This function must be called after all strings was added
// Aho-Corasick algorithm implementation
// Lecture: https://www.youtube.com/watch?v=V7S80KpbQpk&list=LL&index=5&t=2s
template <char AlphaLeft, char AlphaRight>
requires(AlphaRight >= AlphaLeft)
void TAhoCorasick<AlphaLeft, AlphaRight>::buildAutomata() {
  Nodes_[0].SuffixLink = NoPathFlag;
  Nodes_[0].ToTerminalLink = NoPathFlag;
  for (char c = AlphaLeft; c <= AlphaRight; ++c) {
    if (Nodes_[0].Next[c - AlphaLeft] == UndefinedFlag) {
      Nodes_[0].Next[c - AlphaLeft] = 0;
    }
  }
  std::queue<std::size_t> nodes_queue;
  nodes_queue.push(0);
  while (!nodes_queue.empty()) {
    std::size_t parent = nodes_queue.front();
    nodes_queue.pop();
    for (char c = AlphaLeft; c <= AlphaRight; ++c) {
      std::size_t child = Nodes_[parent].Next[c - AlphaLeft];
      if (Nodes_[child].SuffixLink != UndefinedFlag) {
        continue;
      }
      Nodes_[child].SuffixLink =
          (parent == 0 ? 0
                       : Nodes_[Nodes_[parent].SuffixLink].Next[c - AlphaLeft]);
      const std::size_t& suff_link_node = Nodes_[child].SuffixLink;
      Nodes_[child].ToTerminalLink =
          (Nodes_[suff_link_node].IsTerminal
               ? suff_link_node
               : Nodes_[suff_link_node].ToTerminalLink);
      for (char d = AlphaLeft; d <= AlphaRight; ++d) {
        if (Nodes_[child].Next[d - AlphaLeft] != UndefinedFlag) {
          continue;
        }
        Nodes_[child].Next[d - AlphaLeft] =
            Nodes_[Nodes_[child].SuffixLink].Next[d - AlphaLeft];
      }
      nodes_queue.push(child);
    }
  }
}

template <char AlphaLeft, char AlphaRight>
requires(AlphaRight >= AlphaLeft)
TAhoCorasick<AlphaLeft, AlphaRight>::TNode::TNode()
    : IsTerminal(false),
      StrNum(UndefinedFlag),
      SuffixLink(UndefinedFlag),
      ToTerminalLink(UndefinedFlag) {
  std::fill(Next, Next + AlphaSize, UndefinedFlag);
}

////////////////////////////////////////////////////////////////////////////////

}  // namespace NAds::NDs::NAhoCorasick
