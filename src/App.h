#ifndef APP_H
#define APP_H

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
