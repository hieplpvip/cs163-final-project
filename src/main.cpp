#include <string>
#include "App.h"

int main(int argc, char *argv[]) {
  bool verbose = (argc >= 2 && std::string(argv[1]) == "-v");
  App app(verbose);
  app.run();
  return 0;
}
