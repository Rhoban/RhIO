#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include "Common/DataBuffer.hpp"

namespace RhIO {

DataBuffer::DataBuffer(void* data, size_t size) :
    _data((uint8_t*)data),
    _size(size),
    _offset(0)
{
}

size_t DataBuffer::size() const
{
    return _size;
}
size_t DataBuffer::offset() const
{
    return _offset;
}

void DataBuffer::writeType(uint8_t val)
{
    if (_offset + sizeof(uint8_t) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    uint8_t* p = (uint8_t*)(_data + _offset);
    *p = val;
    _offset += sizeof(uint8_t);
}
void DataBuffer::writeBool(bool val)
{
    if (_offset + sizeof(uint8_t) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    uint8_t* p = _data + _offset;
    *p = (val) ? 1 : 0;
    _offset += sizeof(uint8_t);
}
void DataBuffer::writeInt(int64_t val)
{
    if (_offset + sizeof(int64_t) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    int64_t* p = (int64_t*)(_data + _offset);
    *p = val;
    _offset += sizeof(int64_t);
}
void DataBuffer::writeFloat(double val)
{
    if (_offset + sizeof(double) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    double* p = (double*)(_data + _offset);
    *p = val;
    _offset += sizeof(double);
}
void DataBuffer::writeStr(const std::string& val)
{
    writeInt(val.length());
    
    if (_offset + val.length() > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }
    
    memcpy(_data + _offset, val.c_str(), val.length());
    _offset += val.length();
}
void DataBuffer::writeData(const unsigned char* data, size_t size)
{
    writeInt(size);
    
    if (_offset + size > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }
    
    memcpy(_data + _offset, data, size);
    _offset += size;
}
        
uint8_t DataBuffer::readType()
{
    if (_offset + sizeof(uint8_t) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    uint8_t* p = (uint8_t*)(_data + _offset);
    _offset += sizeof(uint8_t);

    return *p;
}
bool DataBuffer::readBool()
{
    if (_offset + sizeof(uint8_t) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    uint8_t* p = _data + _offset;
    _offset += sizeof(uint8_t);

    return (*p > 0) ? true : false;
}
int64_t DataBuffer::readInt()
{
    if (_offset + sizeof(int64_t) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    int64_t* p = (int64_t*)(_data + _offset);
    _offset += sizeof(int64_t);

    return *p;
}
double DataBuffer::readFloat()
{
    if (_offset + sizeof(double) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    double* p = (double*)(_data + _offset);
    _offset += sizeof(double);

    return *p;
}
std::string DataBuffer::readStr()
{
    int64_t len = readInt();
    
    if (_offset + len > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }
    
    std::string str((char*)(_data + _offset), len);
    _offset += len;

    return str;
}
unsigned char* DataBuffer::readData(size_t& size)
{
    size = readInt();
    
    if (_offset + size > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }
    
    unsigned char* data = _data + _offset;
    _offset += size;

    return data;
}
        
void* DataBuffer::data()
{
    return _data;
}

}

