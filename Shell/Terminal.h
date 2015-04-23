#pragma once

#include <string>

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
            static void setColor(std::string name, bool bold=false);
    };
}
