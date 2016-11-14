#include "CSV.h"

namespace RhIO
{
    CSV::CSV(std::ostream *os_)
        : os(os_)
    {
        header = false;
    }
    
    void CSV::push(std::string column, double value)
    {
        if (columns.count(column)) {
            values[columns[column]] = value;
        } else if (!header) {
            int index = columns.size();
            columns[column] = index;
            columnIndexes[index] = column;
            values[index] = value;
        }
    }

    void CSV::newLine()
    {
        if (!header) {
            produceHeader();
            header = true;
        }

        for (unsigned int index=0; index<columns.size(); index++) {
            *os << values[index] << " ";
        }
        *os << std::endl;
        os->flush();
    }

    void CSV::produceHeader()
    {
        for (unsigned int index=0; index<columnIndexes.size(); index++) {
            *os << "# " << (index+1) << ": " << columnIndexes[index] << std::endl;
        }
    }
}
