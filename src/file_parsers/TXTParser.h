#ifndef TXTPARSER_H
#define TXTPARSER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::string, std::vector;

namespace TXTParser {
  bool parseFileToWords(const string &filename, vector<string> &title, vector<string> &content);
};

#endif
