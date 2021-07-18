#ifndef APP_H
#define APP_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "curses_wrapper.h"
#include "parser/TXTParser.h"

using namespace std;

class App {
private:
  void indexFiles();
  void indexStopwords();
  CursesInitializer _curses;

public:
  App();
  ~App();
  void run();
};

#endif
