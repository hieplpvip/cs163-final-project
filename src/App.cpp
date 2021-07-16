#include "App.h"
#include "Global.h"

App::App() {}

App::~App() {}

void App::indexFiles() {
  // Read filenames
  vector<string> files;
  ifstream index_list("data/__index.txt");
  if (!index_list.is_open()) {
    cerr << "Error: Could not open index file.\n";
    exit(1);
  }
  string filename;
  while (index_list >> filename) {
    files.push_back(filename);
  }
  index_list.close();

  // Parse each file
  for (int fileID = 0; fileID < (int)files.size(); ++fileID) {
    cout << "Indexing: " << files[fileID] << '\n';

    vector<string> title, content;
    TXTParser::parseFileToWords("data/" + files[fileID], title, content);
    cout << "Found " << title.size() << " words in title, " << content.size() << " in content\n";

    for (int pos = 0; pos < (int)title.size(); ++pos) {
      Global::trieTitle.addWord(title[pos], fileID, pos);
    }

    for (int pos = 0; pos < (int)content.size(); ++pos) {
      Global::trieContent.addWord(content[pos], fileID, pos);
    }
  }

  cout << "Done indexing " << files.size() << " files!\n";
  cout << "trieTitle has " << Global::trieTitle.numWords << " words\n";
  cout << "trieContent has " << Global::trieContent.numWords << " words\n";
}

void App::run() {
  cout << "Hello World!\n";
  indexFiles();
}
