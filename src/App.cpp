#include "App.h"

App::App() {}

App::~App() {}

void App::indexFiles() {
  // Read filenames
  vector<string> files;
  ifstream indexList("data/__index.txt");
  if (!indexList.is_open()) {
    cerr << "Error: Could not open index file.\n";
    exit(1);
  }
  string filename;
  while (indexList >> filename) {
    files.push_back(filename);
  }
  indexList.close();

  // Parse each file
  for (const string &filename : files) {
    cout << "Indexing: " << filename << '\n';
    vector<string> title, tokens;
    TXTParser::parseFileToTokens("data/" + filename, title, tokens);
    cout << "Found " << title.size() << " tokens in title, " << tokens.size() << " in content\n";
  }
}

void App::run() {
  cout << "Hello World!\n";
  indexFiles();
}
