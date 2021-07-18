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
    if (!TXTParser::parseFileToWords("data/" + files[fileID], title, content)) {
      cerr << "Error: Could not parse file " << files[fileID] << '\n';
      exit(1);
    }
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

void App::indexStopwords() {
  ifstream f("data/stopwords.txt");
  if (!f.is_open()) {
    cerr << "Error: Could not open stopwords file.\n";
    exit(1);
  }

  string stopword;
  while (f >> stopword) {
    Global::trieStopWord.addWord(stopword, -1, -1);
  }

  cout << "Done indexing stopwords!\n";
  cout << "trieStopWord has " << Global::trieStopWord.numWords << " words\n";
}

void App::run() {
  //string name;
  //cout << "Please enter your name: ";
  //setTextColor(TextColor::BLUE);
  //getline(cin, name);
  //setTextColor(TextColor::GREEN);
  //cout << "Hello ";
  //setTextColor(TextColor::BLUE);
  //cout << name << "!\n";
  //setTextColor(TextColor::WHITE);

  indexFiles();
  indexStopwords();
}
