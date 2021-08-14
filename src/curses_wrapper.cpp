#ifndef _WIN32
#include <signal.h>
#endif
#include <iostream>
#include "curses_wrapper.h"

class ncursesbuf : public std::streambuf {
private:
  static constexpr int BUF_SIZE = 1024;

  bool read;
  int n;
  char* buf;

public:
  ncursesbuf() {
    read = false;
    n = 0;
    buf = new char[BUF_SIZE];
  }

  ~ncursesbuf() {
    delete[] buf;
  }

  int_type overflow(int c) {
    printw("%c", c);
    refresh();
    return 1;
  }

  int_type underflow() {
    if (gptr() < egptr()) {
      return traits_type::to_int_type(*gptr());
    }

    char c;
    n = 0;
    while ((buf[n++] = c = getch()) != EOF) {
      if (c == KEY_BACKSPACE || c == KEY_DC || c == 8 || c == 127) {
        buf[--n] = 0;

        if (n > 0) {
          printw("\b \b");
          buf[--n] = 0;
        }

        refresh();
      } else {
        printw("%c", c);
        if (c == '\n') break;
      }
    }

    setg(buf, buf, buf + n);

    return traits_type::to_int_type(*gptr());
  }

  int_type sync() {
    refresh();
    return 0;
  }
};

#ifndef _WIN32
void resizeHandler(int sig) {
  redrawwin(stdscr);
}
#endif

CursesInitializer::CursesInitializer() {
#ifndef _WIN32
  signal(SIGWINCH, resizeHandler);
#endif

  // init ncurses
  initscr();
  cbreak();
  clear();
  noecho();
  scrollok(stdscr, true);
  idlok(stdscr, true);

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);

  // save old buf
  cinbuf = std::cin.rdbuf();
  coutbuf = std::cout.rdbuf();

  // redirect cin/cout to ncurses
  nbuf = new ncursesbuf();
  std::cin.rdbuf(nbuf);
  std::cout.rdbuf(nbuf);
}

CursesInitializer::~CursesInitializer() {
  // restore buf
  std::cin.rdbuf(cinbuf);
  std::cout.rdbuf(coutbuf);
  delete nbuf;

  printw("Good bye! Press any key to exit...\n");
  refresh();
  getch();

  // restore the terminal
  endwin();
}

void curses_setTextColor(TextColor c) {
  attron(COLOR_PAIR(c));
  refresh();
}
