#ifndef APP_H
#define APP_H

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "Utils.h"
#include "file_parsers/TXTParser.h"

using namespace std;

class App {
private:
  void indexFiles();
  void indexStopwords();

  void showLogo();
  void showMenu();

  void processSearch();

public:
  App();
  ~App();
  void run();
};

#endif
