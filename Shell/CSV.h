#ifndef _RHOBAN_CSV_H
#define _RHOBAN_CSV_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>

namespace RhIO
{
    class CSV
    {
        public:
            CSV(std::ostream *os);

            void push(std::string column, double value);
            void newLine();

        protected:
            std::ostream *os;
            bool header;
            std::map<std::string, int> columns;
            std::map<int, std::string> columnIndexes;
            std::map<int, double> values;

            void produceHeader();
    };
}

#endif
