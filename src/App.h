#ifndef APP_H
#define APP_H

#include <string>

class App {
private:
  std::string DATA_DIR = "";

  void indexFiles();
  void indexStopwords();
  void indexSynwords();
  void showLogo();
  void showMenu();

  void processSearch();

public:
  App(bool verbose, bool use_full_data);
  ~App();
  void run();
};

#endif
