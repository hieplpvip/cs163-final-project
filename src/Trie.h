#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <vector>

constexpr int ALPHABET_SIZE = 41;  // must match charToCode in Utils.cpp

struct Occurrence {
  int fileID;
  int pos;
};

struct TrieNode {
  TrieNode* children[ALPHABET_SIZE] = {};
  bool isWord = false;                  // true if there exists a word corresponding to this node
  int wordID = -1;                      // -1 if isWord is false, otherwise the word's ID, starting from 1
  std::vector<Occurrence> occurrences;  // all occurrences of this word in all files
};

struct Trie {
  TrieNode* root;
  int numWords;

  Trie();
  ~Trie();

  void addWord(const std::string& word, int fileID, int pos);
  TrieNode* findWord(const std::string& word);
};

#endif
