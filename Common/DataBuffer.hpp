#ifndef RHIO_DATABUFFER_HPP
#define RHIO_DATABUFFER_HPP

#include <string>

namespace RhIO {

/**
 * DataBuffer
 */
class DataBuffer
{
    public:

        /**
         * Initialization with given already allocated
         * data and size. No deallocation is done.
         */
        DataBuffer(void* data, size_t size);

        /**
         * Return current data size and 
         * current offset cursor
         */
        size_t size() const;
        size_t offset() const;

        /**
         * Write given each type value inside
         * given data buffer end and update cursor
         */
        void writeType(uint8_t val);
        void writeBool(bool val);
        void writeInt(long val);
        void writeFloat(double val);
        void writeStr(const std::string& val);

        /**
         * Read each type into data buffer
         * at current offset and update cursor
         */
        uint8_t readType();
        bool readBool();
        long readInt();
        double readFloat();
        std::string readStr();

        /**
         * Return internal data pointer
         */
        void* data();

    private:

        /**
         * Data buffer
         */
        uint8_t* _data;

        /**
         * Allocated data size
         */
        size_t _size;

        /**
         * Cursor data offset
         */
        size_t _offset;
};

}

#endif

