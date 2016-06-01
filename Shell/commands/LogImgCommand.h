#pragma once

#include <map>
#include <string>
#include <vector>
#include "Command.h"

namespace RhIO
{
    class LogImgCommand : public Command
    {
        public:
            virtual std::string getName();
            virtual std::string getDesc();
            virtual std::string getUsage();
            virtual void process(std::vector<std::string> args);
            void update(std::string name, size_t width, size_t height, 
                unsigned char* data, size_t size);

        protected:
            std::map<std::string, int> ids;
            std::map<std::string, std::string> names;
    };
}
