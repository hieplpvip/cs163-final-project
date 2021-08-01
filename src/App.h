#ifndef APP_H
#define APP_H

class App {
private:
  void indexFiles();
  void indexStopwords();
  void indexSynwords();
  void showLogo();
  void showMenu();

  void processSearch();

public:
  App(bool verbose);
  ~App();
  void run();
};

#endif
