#include <string>
#include "App.h"

int main(int argc, char *argv[]) {
  bool verbose = false, use_full_data = false;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "-v") {
      verbose = true;
    } else if (std::string(argv[i]) == "-fulldata") {
      use_full_data = true;
    }
  }
  App app(verbose, use_full_data);
  app.run();
  return 0;
}
