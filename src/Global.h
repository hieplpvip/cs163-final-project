#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <vector>
#include "Trie.h"
#include "Utils.h"

#define cdebug \
  if (Global::verbose) std::cout

using std::string, std::vector;

namespace Global {
  extern vector<string> filesList;
  extern vector<vector<string>> fileTitleWords, fileContentWords;
  extern int numFiles, numStopWords;

  extern Trie trieTitle, trieContent, trieStopWord;

  extern vector<string> searchHistory;

  extern bool verbose;
};

#endif
