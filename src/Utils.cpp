#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <iostream>
#include <limits>
#include "Utils.h"

/**
 * Convert character to corresponding code.
 *
 * 0 -> 9: '0' -> '9';
 * 10 -> 35: 'a' or 'A' -> 'z' or 'Z'
 * 36: '.'
 * 37: '$'
 * 38: '%'
 * 39: '#'
 * 40: '-'
 *
 * @param c A character.
 *
 * @return Corresponding code if character is valid, otherwise -1.
 */
int charToCode(char c) {
  if ('0' <= c && c <= '9') return c - '0';
  if ('a' <= c && c <= 'z') return c - 'a' + 10;
  if ('A' <= c && c <= 'Z') return c - 'A' + 10;
  if (c == '.') return 36;
  if (c == '$') return 37;
  if (c == '%') return 38;
  if (c == '#') return 39;
  if (c == '-') return 40;
  return -1;
}

void setTextColor(TextColor c) {
#ifdef _WIN32
  switch (c) {
    case TextColor::WHITE:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
      break;
    case TextColor::RED:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
      break;
    case TextColor::GREEN:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
      break;
    case TextColor::YELLOW:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
      break;
    case TextColor::BLUE:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
      break;
    case TextColor::MAGENTA:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
      break;
    case TextColor::CYAN:
      SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
      break;
  }
#else
  switch (c) {
    case TextColor::WHITE:
      std::cout << "\u001b[37m";
      break;
    case TextColor::RED:
      std::cout << "\u001b[31m";
      break;
    case TextColor::GREEN:
      std::cout << "\u001b[32m";
      break;
    case TextColor::YELLOW:
      std::cout << "\u001b[33m";
      break;
    case TextColor::BLUE:
      std::cout << "\u001b[34m";
      break;
    case TextColor::MAGENTA:
      std::cout << "\u001b[35m";
      break;
    case TextColor::CYAN:
      std::cout << "\u001b[36m";
      break;
  }
#endif
}

void testColor() {
  setTextColor(TextColor::WHITE);
  std::cout << "WHITE\n";
  setTextColor(TextColor::RED);
  std::cout << "RED\n";
  setTextColor(TextColor::GREEN);
  std::cout << "GREEN\n";
  setTextColor(TextColor::YELLOW);
  std::cout << "YELLOW\n";
  setTextColor(TextColor::BLUE);
  std::cout << "BLUE\n";
  setTextColor(TextColor::MAGENTA);
  std::cout << "MAGENTA\n";
  setTextColor(TextColor::CYAN);
  std::cout << "CYAN\n";
}

void milliSleep(int milliseconds) {
#ifdef _WIN32
  Sleep(milliseconds);
#else
  usleep(milliseconds * 1000);
#endif
}

void clearScreen() {
#ifdef _WIN32
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD topLeft = {0, 0};
  DWORD dwCount, dwSize;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hOutput, &csbi);
  dwSize = csbi.dwSize.X * csbi.dwSize.Y;
  FillConsoleOutputCharacter(hOutput, 0x20, dwSize, topLeft, &dwCount);
  FillConsoleOutputAttribute(hOutput, 0x07, dwSize, topLeft, &dwCount);
  SetConsoleCursorPosition(hOutput, topLeft);
#else
  std::cout << "\033[2J";
  std::cout << "\033[H";
#endif
}

void waitForEnter() {
  // https://www.daniweb.com/programming/software-development/threads/90228/flushing-the-input-stream
#pragma push_macro("max")
#undef max
  if (std::cin.rdbuf()->sungetc() != std::char_traits<char>::eof() && std::cin.get() != '\n') {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  std::cout << "Press Enter to continue...";
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
#pragma pop_macro("max")
}
