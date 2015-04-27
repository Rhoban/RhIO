#include <stdexcept>
#include "Bind.hpp"

namespace RhIO {
        
Bind::Bind(const std::string& prefixChild) :
    _prefix(prefixChild)
{
    if (
        _prefix != "" && 
        _prefix[_prefix.length()] != '/'
    ) {
        _prefix = _prefix + '/';
    }
}
        
std::unique_ptr<ValueBuilderBool> 
    Bind::bindNew(const std::string& name, bool& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != NoValue) {
        throw std::logic_error(
            "RhIO already created value name: " + name);
    }
    //Add to bind container
    _bindsBool.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
    //Create new value
    return RhIO::Root.newBool(_prefix+name);
}
std::unique_ptr<ValueBuilderInt> 
    Bind::bindNew(const std::string& name, int& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != NoValue) {
        throw std::logic_error(
            "RhIO already created value name: " + name);
    }
    //Add to bind container
    _bindsInt.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
    //Create new value
    return RhIO::Root.newInt(_prefix+name);
}
std::unique_ptr<ValueBuilderInt> 
    Bind::bindNew(const std::string& name, long& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != NoValue) {
        throw std::logic_error(
            "RhIO already created value name: " + name);
    }
    //Add to bind container
    _bindsLong.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
    //Create new value
    return RhIO::Root.newInt(_prefix+name);
}
std::unique_ptr<ValueBuilderFloat> 
    Bind::bindNew(const std::string& name, float& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != NoValue) {
        throw std::logic_error(
            "RhIO already created value name: " + name);
    }
    //Add to bind container
    _bindsFloat.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
    //Create new value
    return RhIO::Root.newFloat(_prefix+name);
}
std::unique_ptr<ValueBuilderFloat> 
    Bind::bindNew(const std::string& name, double& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != NoValue) {
        throw std::logic_error(
            "RhIO already created value name: " + name);
    }
    //Add to bind container
    _bindsDouble.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
    //Create new value
    return RhIO::Root.newFloat(_prefix+name);
}
std::unique_ptr<ValueBuilderStr> 
    Bind::bindNew(const std::string& name, std::string& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != NoValue) {
        throw std::logic_error(
            "RhIO already created value name: " + name);
    }
    //Add to bind container
    _bindsStr.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
    //Create new value
    return RhIO::Root.newStr(_prefix+name);
}

void Bind::bind(const std::string& name, bool& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeBool) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsBool.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
}
void Bind::bind(const std::string& name, int& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeInt) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsInt.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
}
void Bind::bind(const std::string& name, long& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeInt) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsLong.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
}
void Bind::bind(const std::string& name, float& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeFloat) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsFloat.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
}
void Bind::bind(const std::string& name, double& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeFloat) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsDouble.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var});
}
void Bind::bind(const std::string& name, std::string& var)
{
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeStr) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsStr.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
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

