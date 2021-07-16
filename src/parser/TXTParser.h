#ifndef TXTPARSER_H
#define TXTPARSER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace TXTParser {
  bool parseFileToWords(const string &filename, vector<string> &title, vector<string> &content);
};

#endif
