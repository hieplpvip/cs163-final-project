#include <curses.h>
#include <streambuf>

class CursesInitializer {
private:
  std::streambuf *cinbuf, *coutbuf, *nbuf;

public:
  CursesInitializer();
  ~CursesInitializer();
};

enum class TextColor {
  WHITE = 1,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
};

void curses_setTextColor(TextColor c);
