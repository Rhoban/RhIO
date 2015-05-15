#pragma once

#include <map>
#include <string>
#include <Node.h>
#include "Command.h"

#define PAD_AXIS        1
#define PAD_BUTTON      2
#define PAD_TOGGLE      3
#define PAD_INCREMENT   4
#define PAD_DECREMENT   5

namespace RhIO
{
    struct PadIO
    {
        NodeValue node;
        std::string param;
        int number;
        int state;
        int type;
        int value;

        float min, max;
        float step;
        float fvalue;
    };

    class PadCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual void process(std::vector<std::string> args);
            std::map<int, PadIO> import(std::string name);
            void update(PadIO &pad);
    };
}
