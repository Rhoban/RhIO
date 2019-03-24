#pragma once

#include <string>

// normal
#define NRM "\x1B[0m"

//%d for bold
// foreground
#define F_GRE "\x1B[%d;30m"
#define F_RED "\x1B[%d;31m"
#define F_GRN "\x1B[%d;32m"
#define F_YEL "\x1B[%d;33m"
#define F_BLU "\x1B[%d;34m"
#define F_MAG "\x1B[%d;35m"
#define F_CYN "\x1B[%d;36m"
#define F_WHT "\x1B[%d;37m"

// background
#define B_GRE "\x1B[%d;40m"
#define B_RED "\x1B[%d;41m"
#define B_GRN "\x1B[%d;42m"
#define B_YEL "\x1B[%d;43m"
#define B_BLU "\x1B[%d;44m"
#define B_MAG "\x1B[%d;45m"
#define B_CYN "\x1B[%d;46m"
#define B_WHT "\x1B[%d;47m"

namespace RhIO
{
class Terminal
{
public:
  /**
   * Handle format of the terminal
   */
  static void clear();
  static void initCursor();
  static void clearScreen();
  static void clearLine();
  static void cursorRight();
  static void cursorLeft();
  static void cursorNLeft(int n);
  static void cursorNRight(int n);
  static void setColor(std::string name, bool bold = false);
  static void setBColor(std::string name, bool bold = false);
};
}  // namespace RhIO
