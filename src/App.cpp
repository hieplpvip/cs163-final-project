#include "App.h"
#include "Global.h"

App::App() {}

App::~App() {}

void App::indexFiles() {
  setTextColor(TextColor::YELLOW);
  cout << "Begin indexing text files...\n";
  setTextColor(TextColor::WHITE);

  double time = clock();

  // Read filenames
  vector<string> files;
  ifstream index_list("data/__index.txt");
  if (!index_list.is_open()) {
    setTextColor(TextColor::RED);
    cerr << "Error: Could not open index file.\n";
    setTextColor(TextColor::WHITE);
    exit(1);
  }
  string filename;
  while (index_list >> filename) {
    files.push_back(filename);
  }
  index_list.close();

  // Parse each file
  for (int fileID = 0; fileID < (int)files.size(); ++fileID) {
    setTextColor(TextColor::BLUE);
    cout << "Indexing: " << files[fileID] << '\n';
    setTextColor(TextColor::WHITE);

    vector<string> title, content;
    if (!TXTParser::parseFileToWords("data/" + files[fileID], title, content)) {
      setTextColor(TextColor::RED);
      cerr << "Error: Could not parse file " << files[fileID] << '\n';
      setTextColor(TextColor::WHITE);
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

  time = (clock() - time) / CLOCKS_PER_SEC;

  setTextColor(TextColor::GREEN);
  cout << "Done indexing " << files.size() << " files in " << fixed << setprecision(2) << time << " seconds!\n";
  cout << "trieTitle has " << Global::trieTitle.numWords << " words\n";
  cout << "trieContent has " << Global::trieContent.numWords << " words\n";
  setTextColor(TextColor::WHITE);
}

void App::indexStopwords() {
  setTextColor(TextColor::YELLOW);
  cout << "Begin indexing stopwords...\n";
  setTextColor(TextColor::WHITE);

  ifstream f("data/stopwords.txt");
  if (!f.is_open()) {
    setTextColor(TextColor::RED);
    cerr << "Error: Could not open stopwords file.\n";
    setTextColor(TextColor::WHITE);
    exit(1);
  }

  string stopword;
  while (f >> stopword) {
    Global::trieStopWord.addWord(stopword, -1, -1);
  }

  setTextColor(TextColor::GREEN);
  cout << "Done indexing stopwords in " << fixed << setprecision(2) << time << " seconds!\n";
  cout << "trieStopWord has " << Global::trieStopWord.numWords << " words\n";
  setTextColor(TextColor::WHITE);
}

void App::showLogo() {
  setTextColor(TextColor::BLUE);
  cout << "Put some fancy logo here\n";
  setTextColor(TextColor::WHITE);
}

void App::showMenu() {
  while (true) {
    clearScreen();
    showLogo();

    int cmd;
    cout << "0. Quit\n";
    cout << "1. Search\n";
    cout << "Your choice: ";
    cin >> cmd;

    if (cmd == 0) {
      break;
    } else if (cmd == 1) {
      processSearch();
    } else {
      setTextColor(TextColor::RED);
      cout << "Error: Invalid choice\n";
      waitForEnter();
      setTextColor(TextColor::WHITE);
    }
  }
}

void App::processSearch() {
  clearScreen();
  showLogo();

  string query;
  cout << "Search for: ";
  cin.ignore();
  getline(cin, query);
}

void App::run() {
  clearScreen();

  indexFiles();
  cout << "\n\n";
  indexStopwords();

  waitForEnter("Indexing stage finished! Press Enter to continue...");

  showMenu();

  // Reset text color
  setTextColor(TextColor::WHITE);

  waitForEnter("App stopped! Press Enter to exit...");
}
