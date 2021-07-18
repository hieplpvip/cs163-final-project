#ifndef UTILS_H
#define UTILS_H

enum class TextColor {
  WHITE = 1,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
};

int charToCode(char c);

void setTextColor(TextColor c);
void testColor();

void milliSleep(int seconds);
void clearScreen();
void waitForEnter();

#endif
