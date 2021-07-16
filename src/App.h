#ifndef APP_H
#define APP_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "parser/TXTParser.h"

using namespace std;

class App {
private:
  void indexFiles();

public:
  App();
  ~App();
  void run();
};

#endif
