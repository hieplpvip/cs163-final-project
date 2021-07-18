#ifndef APP_H
#define APP_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Utils.h"
#include "parser/TXTParser.h"

using namespace std;

class App {
private:
  void indexFiles();
  void indexStopwords();

public:
  App();
  ~App();
  void run();
};

#endif
