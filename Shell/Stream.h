#pragma once

#include "Shell.h"

namespace RhIO
{
    class Stream
    {
        public:
            Stream(Shell*);

            void boolHandler(const std::string &name, long timestamp, bool val);
            void intHandler(const std::string &name, long timestamp, int val);
            void floatHandler(const std::string &name, long timestamp, float val);
            void stringHandler(const std::string &name, long timestamp, const std::string &val);
    }
}
