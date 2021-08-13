#include "Global.h"

namespace Global {
  vector<string> filesList;
  vector<vector<string>> fileTitleWords, fileContentWords;
  int numFiles = 0, numStopWords = 0, numSynGroups = 0;

  Trie trieTitle, trieContent, trieStopWord, trieSynonym;

  vector<string> searchHistory;

  vector<vector<string>> synGroups;

  bool verbose = false;
};
