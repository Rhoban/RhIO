#include <stdexcept>
#include "Bind.hpp"

namespace RhIO {

void Bind::bind(const std::string& name, bool& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(name) != TypeBool) {
        RhIO::Root.newBool(name);
    }
    _bindsBool.push_back({
        relativeName(name), 
        getChildPtr(name), 
        &var});
}
void Bind::bind(const std::string& name, int& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(name) != TypeInt) {
        RhIO::Root.newInt(name);
    }
    _bindsInt.push_back({
        relativeName(name), 
        getChildPtr(name), 
        &var});
}
void Bind::bind(const std::string& name, long& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(name) != TypeInt) {
        RhIO::Root.newInt(name);
    }
    _bindsLong.push_back({
        relativeName(name), 
        getChildPtr(name), 
        &var});
}
void Bind::bind(const std::string& name, float& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(name) != TypeFloat) {
        RhIO::Root.newFloat(name);
    }
    _bindsFloat.push_back({
        relativeName(name), 
        getChildPtr(name), 
        &var});
}
void Bind::bind(const std::string& name, double& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(name) != TypeFloat) {
        RhIO::Root.newFloat(name);
    }
    _bindsDouble.push_back({
        relativeName(name), 
        getChildPtr(name), 
        &var});
}
void Bind::bind(const std::string& name, std::string& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(name) != TypeStr) {
        RhIO::Root.newStr(name);
    }
    _bindsStr.push_back({
        relativeName(name), 
        getChildPtr(name), 
        &var});
}

void Bind::pull()
{
    for (auto& b : _bindsBool) {
        *(b.ptr) = b.node->getBool(b.name);
    }
    for (auto& b : _bindsInt) {
        *(b.ptr) = b.node->getInt(b.name);
    }
    for (auto& b : _bindsLong) {
        *(b.ptr) = b.node->getInt(b.name);
    }
    for (auto& b : _bindsFloat) {
        *(b.ptr) = b.node->getFloat(b.name);
    }
    for (auto& b : _bindsDouble) {
        *(b.ptr) = b.node->getFloat(b.name);
    }
    for (auto& b : _bindsStr) {
        *(b.ptr) = b.node->getStr(b.name);
    }
}

void Bind::push()
{
    for (auto& b : _bindsBool) {
        b.node->setBool(b.name, *(b.ptr));
    }
    for (auto& b : _bindsInt) {
        b.node->setInt(b.name, *(b.ptr));
    }
    for (auto& b : _bindsLong) {
        b.node->setInt(b.name, *(b.ptr));
    }
    for (auto& b : _bindsFloat) {
        b.node->setFloat(b.name, *(b.ptr));
    }
    for (auto& b : _bindsDouble) {
        b.node->setFloat(b.name, *(b.ptr));
    }
    for (auto& b : _bindsStr) {
        b.node->setStr(b.name, *(b.ptr));
    }
}
        
IONode* Bind::getChildPtr(const std::string& name) const
{
    //Extract child absolute path
    size_t pos = name.find_last_of("/");
    if (pos != std::string::npos) {
        std::string path = name.substr(0, pos);
        return &(RhIO::Root.child(path));
    } else {
        return &(RhIO::Root);
    }
}
        
std::string Bind::relativeName(const std::string& name) const
{
    //Extract relative value name
    size_t pos = name.find_last_of("/");
    if (pos != std::string::npos) {
        return name.substr(pos+1);
    } else {
        return name;
    }
}

}

