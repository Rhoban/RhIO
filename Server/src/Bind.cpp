#include <stdexcept>
#include "rhio_server/Bind.hpp"

namespace RhIO {
        
Bind::Bind(const std::string& prefixChild) :
    _prefix(prefixChild)
{
    if (_prefix[_prefix.length()] != '/') {
        _prefix = _prefix + '/';
    }
    
    //Create non existing hierarchy
    createPath(_prefix);
}
        
std::unique_ptr<ValueBuilderBool> Bind::bindNew(
    const std::string& name, bool& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Add to bind container
    _bindsBool.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
    //Create new value
    return RhIO::Root.newBool(_prefix+name);
}
std::unique_ptr<ValueBuilderInt> Bind::bindNew(
    const std::string& name, int& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Add to bind container
    _bindsInt.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
    //Create new value
    return RhIO::Root.newInt(_prefix+name);
}
std::unique_ptr<ValueBuilderInt> Bind::bindNew(
    const std::string& name, long& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Add to bind container
    _bindsLong.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
    //Create new value
    return RhIO::Root.newInt(_prefix+name);
}
std::unique_ptr<ValueBuilderFloat> Bind::bindNew(
    const std::string& name, float& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Add to bind container
    _bindsFloat.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
    //Create new value
    return RhIO::Root.newFloat(_prefix+name);
}
std::unique_ptr<ValueBuilderFloat> Bind::bindNew(
    const std::string& name, double& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Add to bind container
    _bindsDouble.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
    //Create new value
    return RhIO::Root.newFloat(_prefix+name);
}
std::unique_ptr<ValueBuilderStr> Bind::bindNew(
    const std::string& name, std::string& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Create non existing hierarchy
    createPath(_prefix+name);
    //Add to bind container
    _bindsStr.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
    //Create new value
    return RhIO::Root.newStr(_prefix+name);
}

void Bind::bind(const std::string& name, bool& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeBool) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsBool.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
}
void Bind::bind(const std::string& name, int& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeInt) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsInt.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
}
void Bind::bind(const std::string& name, long& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeInt) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsLong.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
}
void Bind::bind(const std::string& name, float& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeFloat) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsFloat.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
}
void Bind::bind(const std::string& name, double& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeFloat) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsDouble.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
}
void Bind::bind(const std::string& name, std::string& var,
    Policy policy)
{
    //Check that variable is not already registered
    if (checkIsRegistered(&var)) {
        throw std::logic_error(
            "RhIO::Bind: Variable address already bind: " + name);
    }
    //Check values exist
    if (RhIO::Root.getValueType(_prefix+name) != TypeStr) {
        throw std::logic_error(
            "RhIO not created value name: " + name);
    }
    //Add to bind container
    _bindsStr.push_back({
        relativeName(_prefix+name), 
        getChildPtr(_prefix+name), 
        &var,
        policy});
}

const IONode& Bind::node() const
{
    if (_prefix == "") {
        return RhIO::Root;
    } else {
        return *(getChildPtr(_prefix));
    }
}
IONode& Bind::node()
{
    if (_prefix == "") {
        return RhIO::Root;
    } else {
        return *(getChildPtr(_prefix));
    }
}

void Bind::newStream(const std::string& name, 
    const std::string& comment)
{
    //Create non existing hierarchy
    createPath(_prefix+name);

    node().newStream(name, comment);
}
std::ostream& Bind::out(const std::string& name)
{
    return node().out(name);
}

void Bind::newFrame(const std::string& name, 
    const std::string& comment,
    FrameFormat format)
{
    //Create non existing hierarchy
    createPath(_prefix+name);
    
    node().newFrame(name, comment, format);
}
bool Bind::frameIsStreaming(const std::string& name) const
{
    return node().frameIsStreaming(name);
}
void Bind::framePush(const std::string& name, 
    size_t width, size_t height,
    unsigned char* data, size_t size)
{
    node().framePush(name, width, height, data, size);
}

void Bind::pull()
{
    for (auto& b : _bindsBool) {
        if (b.policy == PushAndPull || b.policy == PullOnly) {
            *(b.ptr) = b.node->getBool(b.name);
        }
    }
    for (auto& b : _bindsInt) {
        if (b.policy == PushAndPull || b.policy == PullOnly) {
            *(b.ptr) = b.node->getInt(b.name);
        }
    }
    for (auto& b : _bindsLong) {
        if (b.policy == PushAndPull || b.policy == PullOnly) {
            *(b.ptr) = b.node->getInt(b.name);
        }
    }
    for (auto& b : _bindsFloat) {
        if (b.policy == PushAndPull || b.policy == PullOnly) {
            *(b.ptr) = b.node->getFloat(b.name);
        }
    }
    for (auto& b : _bindsDouble) {
        if (b.policy == PushAndPull || b.policy == PullOnly) {
            *(b.ptr) = b.node->getFloat(b.name);
        }
    }
    for (auto& b : _bindsStr) {
        if (b.policy == PushAndPull || b.policy == PullOnly) {
            *(b.ptr) = b.node->getStr(b.name);
        }
    }
}

void Bind::push()
{
    for (auto& b : _bindsBool) {
        if (b.policy == PushAndPull || b.policy == PushOnly) {
            b.node->setBool(b.name, *(b.ptr));
        }
    }
    for (auto& b : _bindsInt) {
        if (b.policy == PushAndPull || b.policy == PushOnly) {
            b.node->setInt(b.name, *(b.ptr));
        }
    }
    for (auto& b : _bindsLong) {
        if (b.policy == PushAndPull || b.policy == PushOnly) {
            b.node->setInt(b.name, *(b.ptr));
        }
    }
    for (auto& b : _bindsFloat) {
        if (b.policy == PushAndPull || b.policy == PushOnly) {
            b.node->setFloat(b.name, *(b.ptr));
        }
    }
    for (auto& b : _bindsDouble) {
        if (b.policy == PushAndPull || b.policy == PushOnly) {
            b.node->setFloat(b.name, *(b.ptr));
        }
    }
    for (auto& b : _bindsStr) {
        if (b.policy == PushAndPull || b.policy == PushOnly) {
            b.node->setStr(b.name, *(b.ptr));
        }
    }
}
        
void Bind::createPath(const std::string& path)
{
    if (path == "" || path == "/") return;

    //Extract absolute path
    std::string tmp = path;
    size_t pos = tmp.find_last_of("/");
    if (pos != std::string::npos) {
        tmp = tmp.substr(0, pos);
    }

    //Create no existing path
    if (!RhIO::Root.childExist(tmp)) {
        RhIO::Root.newChild(tmp);
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
        
bool Bind::checkIsRegistered(const void* addr) const
{
    for (size_t i=0;i<_bindsBool.size();i++) {
        if (_bindsBool[i].ptr == (bool*)addr) {
            return true;
        }
    }
    for (size_t i=0;i<_bindsInt.size();i++) {
        if (_bindsInt[i].ptr == (int*)addr) {
            return true;
        }
    }
    for (size_t i=0;i<_bindsLong.size();i++) {
        if (_bindsLong[i].ptr == (long*)addr) {
            return true;
        }
    }
    for (size_t i=0;i<_bindsFloat.size();i++) {
        if (_bindsFloat[i].ptr == (float*)addr) {
            return true;
        }
    }
    for (size_t i=0;i<_bindsDouble.size();i++) {
        if (_bindsDouble[i].ptr == (double*)addr) {
            return true;
        }
    }
    for (size_t i=0;i<_bindsStr.size();i++) {
        if (_bindsStr[i].ptr == (std::string*)addr) {
            return true;
        }
    }
    return false;
}

}

