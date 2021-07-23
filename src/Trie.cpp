#include "Trie.h"
#include "Utils.h"

void cleanMemory(TrieNode* node) {
  if (node == nullptr) return;
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    cleanMemory(node->children[i]);
  }
  delete node;
}

Trie::Trie() {
  root = nullptr;
  numWords = 0;
}

Trie::~Trie() {
  cleanMemory(root);
  root = nullptr;
  numWords = 0;
}

void Trie::addWord(const std::string& word, int fileID, int pos) {
  TrieNode* cur = root;
  bool not_empty_word = false;

  for (char c : word) {
    int code = charToCode(c);
    if (code == -1) continue;
    not_empty_word = true;

    if (cur == nullptr) {
      // trie is empty, create a root node
      root = cur = new TrieNode();
    }

    if (cur->children[code] == nullptr) {
      // create a new child node
      cur->children[code] = new TrieNode();
    }
    cur = cur->children[code];
  }

  if (not_empty_word) {
    if (!cur->isWord) {
      // new word, increase numWords and set wordID
      cur->isWord = true;
      cur->wordID = ++numWords;
    }

    cur->occurrences.emplace_back(fileID, pos);
  }
}

TrieNode* Trie::findWord(const std::string& word) {
  if (root == nullptr) {
    return nullptr;
  }

  TrieNode* cur = root;
  bool not_empty_word = false;

  for (char c : word) {
    int code = charToCode(c);
    if (code == -1) continue;
    not_empty_word = true;

    cur = cur->children[code];
    if (cur == nullptr) {
      // word not found in trie
      return nullptr;
    }
  }

  return not_empty_word ? cur : nullptr;
}
