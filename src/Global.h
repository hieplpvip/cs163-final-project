#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <unordered_set>
#include <vector>
#include "Trie.h"
#include "Utils.h"

#define cdebug \
  if (Global::verbose) std::cout

using std::string, std::vector;

namespace Global {
  extern std::unordered_set<string> filesSet;
  extern vector<string> filesList;
  extern vector<vector<string>> fileTitleWords, fileContentWords;
  extern int numFiles, numStopWords, numSynGroups;

  extern Trie trieTitle, trieContent, trieStopWord, trieSynonym;

  extern vector<string> searchHistory;

  extern vector<vector<string>> synGroups;

  extern bool verbose;
};

#endif
