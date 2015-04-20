#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include "DataBuffer.hpp"

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
void DataBuffer::writeInt(long val)
{
    if (_offset + sizeof(long) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    long* p = (long*)(_data + _offset);
    *p = val;
    _offset += sizeof(long);
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
long DataBuffer::readInt()
{
    if (_offset + sizeof(long) > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }

    long* p = (long*)(_data + _offset);
    _offset += sizeof(long);

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
    long len = readInt();
    
    if (_offset + len > _size) {
        throw std::logic_error("RhIO buffer size overflow");
    }
    
    std::string str((char*)(_data + _offset), len);
    _offset += len;

    return str;
}
        
void* DataBuffer::data()
{
    return _data;
}

}

