#include "TXTParser.h"

/**
 * Parse TXT file to words.
 *
 * @param filename Filename of TXT file to parse.
 * @param title A `vector` to store parsed title words.
 * @param content A `vector` to store parsed content words.
 *
 * @return `true` if the file is parsed successfully, otherwise `false`.
 */
bool TXTParser::parseFileToWords(const string &filename, vector<string> &title, vector<string> &content) {
  ifstream f(filename);
  if (!f.is_open()) {
    // Could not open file
    return false;
  }

  title.clear();
  content.clear();

  // FIXME: Some files don't have title but only one line of content.
  // For now just ignore the title and treat everything as content.

  string first_line, word;
  //getline(f, first_line);
  //stringstream ss(first_line);
  //while (ss >> word) {
  //  title.emplace_back(word);
  //}
  while (f >> word) {
    content.emplace_back(word);
  }

  return true;
}
