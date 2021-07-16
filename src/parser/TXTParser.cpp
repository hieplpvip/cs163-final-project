#include "TXTParser.h"

/**
 * Parse TXT file to tokens.
 *
 * @param filename Filename of TXT file to parse.
 * @param title A `vector` to store parsed title tokens.
 * @param tokens A `vector` to store parsed content tokens.
 *
 * @return `true` if the file is parsed successfully, otherwise `false`.
 */
bool TXTParser::parseFileToTokens(const string &filename, vector<string> &title, vector<string> &tokens) {
  ifstream f(filename);
  if (!f.is_open()) {
    // Could not open file
    return false;
  }

  title.clear();
  tokens.clear();

  string first_line, token;
  getline(f, first_line);
  stringstream ss(first_line);
  while (ss >> token) {
    title.emplace_back(token);
  }
  while (f >> token) {
    tokens.emplace_back(token);
  }

  return true;
}
