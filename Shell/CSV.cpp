#include "CSV.h"

namespace RhIO
{
    CSV::CSV()
    {
    }

    void CSV::open(std::string filename)
    {
        ofs.open(filename.c_str());
        ofs.precision(10);
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
            ofs << values[index] << " ";
        }
        ofs << std::endl;
        ofs.flush();
    }

    void CSV::produceHeader()
    {
        for (unsigned int index=0; index<columnIndexes.size(); index++) {
            ofs << "# " << (index+1) << ": " << columnIndexes[index] << std::endl;
        }
    }
    
    void CSV::close()
    {
        ofs.close();
    }

}
