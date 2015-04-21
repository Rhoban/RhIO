#include "Terminal.h"

namespace RhIO
{
    void Terminal::clear()
    {
#ifdef HAS_COLORS
        printf("\x1b[0m");
#endif
    }

    void Terminal::setColor(std::string name, bool bold)
    {
#ifdef HAS_COLORS
        if (name == "red") {
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
}
