#include "Global.h"

namespace Global {
  vector<string> filesList;
  vector<vector<string>> fileTitleWords, fileContentWords;
  int numFiles = 0, numStopWords = 0, numSynWords = 0;

  Trie trieTitle, trieContent, trieStopWord, trieSynWord;

  vector<string> searchHistory;

  bool verbose = false;
};
