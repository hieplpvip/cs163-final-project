#include <string>
#include "App.h"

int main(int argc, char *argv[]) {
  bool verbose = false, use_full_data = false;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-v") {
      verbose = true;
    }
  }
  App app(verbose);
  app.run();
  return 0;
}
