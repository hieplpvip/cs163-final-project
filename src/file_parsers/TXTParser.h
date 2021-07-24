#ifndef TXTPARSER_H
#define TXTPARSER_H

#include <string>
#include <vector>

using std::string, std::vector;

namespace TXTParser {
  bool parseFileToWords(const string &filename, vector<string> &title, vector<string> &content);
};

#endif
