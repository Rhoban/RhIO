#include "Terminal.h"

namespace RhIO
{
    void Terminal::clear()
    {
#ifdef HAS_COLORS
        printf("\x1b[0m");
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
        if (name == "white") {
            printf("\x1b[%d;37m", bold);
        } else if (name == "red") {
            printf("\x1b[%d;31m", bold);
        } else if (name == "green") {
            printf("\x1b[%d;32m", bold);
        } else if (name == "yellow") {
            printf("\x1b[%d;33m", bold);
        } else if (name == "blue") {
            printf("\x1b[%d;34m", bold);
        } else if (name == "magenta") {
            printf("\x1b[%d;35m", bold);
        } else if (name == "darkblue") {
            printf("\x1b[%d;36m", bold);
        } else if (name == "grey") {
            printf("\x1b[%d;30m", bold);
        } else  {
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
        printf("\033[%dD",n);
    }
    void Terminal::cursorNRight(int n)
    {
        printf("\033[%dC",n);
    }
}
