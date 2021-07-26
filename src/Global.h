#ifndef GLOBAL_H
#define GLOBAL_H

#include "Trie.h"
#include "Utils.h"

#define cdebug \
  if (Global::verbose) std::cout

namespace Global {
  extern Trie trieTitle, trieContent, trieStopWord;
  extern int numFiles, numStopWords;
  extern bool verbose;
};

#endif
