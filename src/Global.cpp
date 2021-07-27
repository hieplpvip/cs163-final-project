#include "Global.h"

namespace Global {
  Trie trieTitle, trieContent, trieStopWord;
  vector<string> filesList;
  vector<vector<string>> fileTitleWords, fileContentWords;
  int numFiles = 0, numStopWords = 0;
  bool verbose = false;
};
