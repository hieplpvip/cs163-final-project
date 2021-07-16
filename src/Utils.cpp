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
