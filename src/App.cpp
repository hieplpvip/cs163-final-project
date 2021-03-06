#include "App.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Engine.h"
#include "Global.h"
#include "Utils.h"
#include "file_parsers/TXTParser.h"

using std::cin, std::cout, std::cerr;
using std::string, std::vector;

App::App(bool verbose) {
  Global::numFiles = 0;
  Global::numStopWords = 0;
  Global::verbose = verbose;
}

App::~App() {}

void App::indexFiles() {
  setTextColor(TextColor::YELLOW);
  cout << "Begin indexing text files...\n";
  setTextColor(TextColor::WHITE);

  double time = clock();

  // Read filenames
  std::ifstream index_list(DATA_DIR + "___index.txt");
  if (!index_list.is_open()) {
    setTextColor(TextColor::RED);
    cerr << "Error: Could not open index file.\n";
    setTextColor(TextColor::WHITE);
    exit(1);
  }
  string filename;
  int numNewFiles = 0;
  while (getline(index_list, filename)) {
    if (!Global::filesSet.count(filename)) {
      ++numNewFiles;
      Global::filesSet.insert(filename);
      Global::filesList.push_back(filename);
    }
  }
  index_list.close();

  // Parse each file
  Global::numFiles = Global::filesList.size();
  Global::fileTitleWords.resize(Global::numFiles);
  Global::fileContentWords.resize(Global::numFiles);
  for (int fileID = Global::numFiles - numNewFiles; fileID < Global::numFiles; ++fileID) {
    const string &filename = Global::filesList[fileID];

    setTextColor(TextColor::BLUE);
    cdebug << "Indexing: " << filename << '\n';
    setTextColor(TextColor::WHITE);

    auto &title = Global::fileTitleWords[fileID];
    auto &content = Global::fileContentWords[fileID];
    if (!TXTParser::parseFileToWords(DATA_DIR + filename, title, content)) {
      //setTextColor(TextColor::RED);
      //cerr << "Error: Could not parse file " << filename << '\n';
      //setTextColor(TextColor::WHITE);
      //exit(1);
      continue;
    }
    cdebug << "Found " << title.size() << " words in title, " << content.size() << " in content\n";

    for (int pos = 0; pos < (int)title.size(); ++pos) {
      Global::trieTitle.addWord(title[pos], fileID, pos);
    }

    for (int pos = 0; pos < (int)content.size(); ++pos) {
      Global::trieContent.addWord(content[pos], fileID, pos);
    }
  }

  time = (clock() - time) / CLOCKS_PER_SEC;

  setTextColor(TextColor::GREEN);
  cout << "Done indexing " << numNewFiles << " files in " << std::fixed << std::setprecision(2) << time << " seconds!\n";
  cout << "trieTitle has " << Global::trieTitle.numWords << " words\n";
  cout << "trieContent has " << Global::trieContent.numWords << " words\n\n";
  setTextColor(TextColor::WHITE);
}

// insert synonyms into tree
void App::indexSynwords() {
  setTextColor(TextColor::YELLOW);
  cout << "Begin indexing synonyms...\n";
  setTextColor(TextColor::WHITE);

  std::ifstream f("synonyms/synonyms.txt");
  if (!f.is_open()) {
    setTextColor(TextColor::RED);
    cerr << "Error: Could not open synonyms file.\n";
    setTextColor(TextColor::WHITE);
    exit(1);
  }

  string line;
  string tmp;
  string key = "";
  while (getline(f, line)) {
    if (line[0] == '=') {
      key.clear();
      continue;
    }

    std::stringstream ss(line);
    ss >> tmp;
    if (tmp == "KEY:") {
      ss >> key;
    } else if (tmp == "SYN:") {
      if (key.empty() || Global::trieSynonym.findWord(key) != nullptr) {
        continue;
      }

      Global::trieSynonym.addWord(key, Global::numSynGroups++, -1);
      Global::synGroups.push_back({});
      Global::synGroups.back().push_back(key);

      while (ss >> tmp) {
        Global::synGroups.back().push_back(tmp);
      }

      key.clear();
    } else if (tmp != "ANT:") {
      assert(false);
    }
  }

  setTextColor(TextColor::GREEN);
  cout << "Done indexing synonyms in " << std::fixed << std::setprecision(2) << time << " seconds!\n";
  cout << "trieSynonym has " << Global::trieSynonym.numWords << " words\n\n";
  setTextColor(TextColor::WHITE);
}

void App::indexStopwords() {
  setTextColor(TextColor::YELLOW);
  cout << "Begin indexing stopwords...\n";
  setTextColor(TextColor::WHITE);

  std::ifstream f("stopwords/stopwords.txt");
  if (!f.is_open()) {
    setTextColor(TextColor::RED);
    cerr << "Error: Could not open stopwords file.\n";
    setTextColor(TextColor::WHITE);
    exit(1);
  }

  string stopword;
  while (f >> stopword) {
    ++Global::numStopWords;
    Global::trieStopWord.addWord(stopword, -1, -1);
  }

  setTextColor(TextColor::GREEN);
  cout << "Done indexing stopwords in " << std::fixed << std::setprecision(2) << time << " seconds!\n";
  cout << "trieStopWord has " << Global::trieStopWord.numWords << " words\n\n";
  setTextColor(TextColor::WHITE);
}

void App::showLogo() {
  setTextColor(TextColor::BLUE);
  cout << "   _____                       ";
  setTextColor(TextColor::GREEN);
  cout << "_      \n";

  setTextColor(TextColor::BLUE);
  cout << "  / ____|                     ";
  setTextColor(TextColor::GREEN);
  cout << "| |     \n";

  setTextColor(TextColor::BLUE);
  cout << " | |  __   ";
  setTextColor(TextColor::RED);
  cout << "___    ";
  setTextColor(TextColor::YELLOW);
  cout << "___    ";
  setTextColor(TextColor::BLUE);
  cout << "__ _ ";
  setTextColor(TextColor::GREEN);
  cout << "| |  ";
  setTextColor(TextColor::RED);
  cout << "___ \n";

  setTextColor(TextColor::BLUE);
  cout << " | | |_ | ";
  setTextColor(TextColor::RED);
  cout << "/ _ \\  ";
  setTextColor(TextColor::YELLOW);
  cout << "/ _ \\  ";
  setTextColor(TextColor::BLUE);
  cout << "/ _` |";
  setTextColor(TextColor::GREEN);
  cout << "| | ";
  setTextColor(TextColor::RED);
  cout << "/ _ \\\n";

  setTextColor(TextColor::BLUE);
  cout << " | |__| |";
  setTextColor(TextColor::RED);
  cout << "| (_) |";
  setTextColor(TextColor::YELLOW);
  cout << "| (_) |";
  setTextColor(TextColor::BLUE);
  cout << "| (_| |";
  setTextColor(TextColor::GREEN);
  cout << "| |";
  setTextColor(TextColor::RED);
  cout << "|  __/\n";

  setTextColor(TextColor::BLUE);
  cout << "  \\_____| ";
  setTextColor(TextColor::RED);
  cout << "\\___/  ";
  setTextColor(TextColor::YELLOW);
  cout << "\\___/  ";
  setTextColor(TextColor::BLUE);
  cout << "\\__, |";
  setTextColor(TextColor::GREEN);
  cout << "|_| ";
  setTextColor(TextColor::RED);
  cout << "\\___|\n ";

  setTextColor(TextColor::BLUE);
  cout << "                        __/ |       \n";

  setTextColor(TextColor::BLUE);
  cout << "                        |___/        \n";

  cout << '\n';
  setTextColor(TextColor::WHITE);
}

void App::showMenu() {
  while (true) {
    clearScreen();
    showLogo();

    int cmd;
    setTextColor(TextColor::RED);
    cout << "0. Quit\n";
    setTextColor(TextColor::BLUE);
    cout << "1. Search\n";
    setTextColor(TextColor::GREEN);
    cout << "2. Show history\n";
    setTextColor(TextColor::YELLOW);
    cout << "3. Clear history\n";
    setTextColor(TextColor::MAGENTA);
    cout << "4. Index new data files\n";
    setTextColor(TextColor::WHITE);
    cout << "Your choice: ";
    cin >> cmd;

    clearScreen();
    showLogo();

    if (cmd == 0) {
      break;
    } else if (cmd == 1) {
      processSearch();
    } else if (cmd == 2) {
      if (Global::searchHistory.empty()) {
        setTextColor(TextColor::RED);
        cout << "You haven't searched for anything!\n";
        setTextColor(TextColor::WHITE);
      } else {
        setTextColor(TextColor::BLUE);
        cout << "You have searched for:\n";
        setTextColor(TextColor::WHITE);
        for (int i = 0; i < (int)Global::searchHistory.size(); ++i) {
          cout << (i + 1) << ". " << Global::searchHistory[i] << '\n';
        }
      }
      cout << '\n';
      waitForEnter();
    } else if (cmd == 3) {
      Global::searchHistory.clear();
      setTextColor(TextColor::BLUE);
      cout << "History cleared\n\n";
      setTextColor(TextColor::WHITE);
      waitForEnter();
    } else if (cmd == 4) {
      indexFiles();
      setTextColor(TextColor::BLUE);
      waitForEnter("Indexing stage finished! Press Enter to continue...");
      setTextColor(TextColor::WHITE);
    } else {
      setTextColor(TextColor::RED);
      cout << "Error: Invalid choice\n\n";
      waitForEnter();
      setTextColor(TextColor::WHITE);
    }
  }
}

void App::processSearch() {
  string query;
  setTextColor(TextColor::BLUE);
  cout << "Search for: ";
  setTextColor(TextColor::WHITE);
  cin.ignore();
  getline(cin, query);

  while (true) {
    clearScreen();
    showLogo();
    setTextColor(TextColor::BLUE);
    cout << "You have entered: ";
    setTextColor(TextColor::WHITE);
    cout << query << '\n';
    setTextColor(TextColor::RED);
    cout << "0. Go back to main menu.\n";
    setTextColor(TextColor::BLUE);
    cout << "1. Begin searching\n";
    setTextColor(TextColor::GREEN);
    cout << "2. Show suggestions\n";
    setTextColor(TextColor::WHITE);

    int cmd;
    cin >> cmd;
    if (cmd == 0) {
      return;
    } else if (cmd == 1) {
      break;
    } else if (cmd == 2) {
      clearScreen();
      showLogo();

      setTextColor(TextColor::BLUE);
      cout << "You have entered: ";
      setTextColor(TextColor::WHITE);
      cout << query << '\n';

      vector<string> suggestions;
      for (string &s : Global::searchHistory) {
        int pos = s.find(query);
        if (pos != -1) {
          suggestions.push_back(s);
        }
      }

      if (suggestions.empty()) {
        setTextColor(TextColor::RED);
        cout << "No suggestions!\n\n";
        setTextColor(TextColor::WHITE);
        waitForEnter();
      } else {
        setTextColor(TextColor::GREEN);
        cout << "Suggestions:\n";
        setTextColor(TextColor::WHITE);

        for (int i = 0; i < (int)suggestions.size(); ++i) {
          cout << (i + 1) << ". " << suggestions[i] << '\n';
        }

        int choice;
        cin >> choice;
        if (choice < 1 || choice > (int)suggestions.size()) {
          cout << "Invalid choice\n";
          waitForEnter();
        } else {
          query = suggestions[choice - 1];
          break;
        }
      }
    } else {
      cout << "Invalid choice\n";
      waitForEnter();
    }
  }

  double time = clock();
  vector<Engine::QueryResult> res;
  Engine::processQuery(query, res);
  time = (clock() - time) / CLOCKS_PER_SEC;

  clearScreen();
  showLogo();
  Engine::displayQueryResult(query, res, time);

  if (std::find(Global::searchHistory.begin(), Global::searchHistory.end(), query) == Global::searchHistory.end()) {
    Global::searchHistory.push_back(query);
  }
}

void App::run() {
  clearScreen();
  showLogo();

  int choice;
  cout << "1. Full dataset\n";
  cout << "2. Small dataset\n";
  cin >> choice;

  if (choice == 1) {
    DATA_DIR = "data-full/";
  } else if (choice == 2) {
    DATA_DIR = "data-small/";
  } else {
    cout << "Invalid choice!\n";
    return;
  }

  clearScreen();
  showLogo();
  indexFiles();
  indexSynwords();
  indexStopwords();

  setTextColor(TextColor::BLUE);
  waitForEnter("Indexing stage finished! Press Enter to continue...");
  setTextColor(TextColor::WHITE);

  showMenu();

  // Reset text color
  setTextColor(TextColor::WHITE);

  waitForEnter("Good bye! Press Enter to exit...");
}
