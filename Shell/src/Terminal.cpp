#include "Terminal.h"

namespace RhIO
{
void Terminal::clear()
{
#ifdef HAS_COLORS
  printf(NRM);
#endif
}

void Terminal::clearScreen()
{
  printf("\033[2J");
}

void Terminal::initCursor()
{
  printf("\033[1;1H");
}

void Terminal::setColor(std::string name, bool bold)
{
#ifdef HAS_COLORS
  if (name == "white")
  {
    printf(F_WHT, bold);
  }
  else if (name == "red")
  {
    printf(F_RED, bold);
  }
  else if (name == "green")
  {
    printf(F_GRN, bold);
  }
  else if (name == "yellow")
  {
    printf(F_YEL, bold);
  }
  else if (name == "blue")
  {
    printf(F_BLU, bold);
  }
  else if (name == "magenta")
  {
    printf(F_MAG, bold);
  }
  else if (name == "darkblue")
  {
    printf(F_CYN, bold);
  }
  else if (name == "grey")
  {
    printf(F_GRE, bold);
  }
  else
  {
    printf("\x1b[%dm", bold);
  }
#endif
}

void Terminal::setBColor(std::string name, bool bold)
{
#ifdef HAS_COLORS
  if (name == "white")
  {
    printf(B_WHT, bold);
  }
  else if (name == "red")
  {
    printf(B_RED, bold);
  }
  else if (name == "green")
  {
    printf(B_GRN, bold);
  }
  else if (name == "yellow")
  {
    printf(B_YEL, bold);
  }
  else if (name == "blue")
  {
    printf(B_BLU, bold);
  }
  else if (name == "magenta")
  {
    printf(B_MAG, bold);
  }
  else if (name == "darkblue")
  {
    printf(B_CYN, bold);
  }
  else if (name == "grey")
  {
    printf(B_GRE, bold);
  }
  else
  {
    printf("\x1b[%dm", bold);
  }
#endif
}

void Terminal::clearLine()
{
  printf("%c[2K\r", 27);
}

// - Move the cursor forward N columns:
//   \033[<N>C
// - Move the cursor backward N columns:
//   \033[<N>D

void Terminal::cursorRight()
{
  printf("\033[C");
}

void Terminal::cursorLeft()
{
  printf("\033[D");
}
void Terminal::cursorNLeft(int n)
{
  printf("\033[%dD", n);
}
void Terminal::cursorNRight(int n)
{
  printf("\033[%dC", n);
}
}  // namespace RhIO
