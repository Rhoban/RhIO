#include "StreamNode.hpp"
#include "IONode.hpp"

namespace RhIO {

StreamNode& StreamNode::operator=(const StreamNode& node)
{
    _streams = node._streams;

    return *this;
}
        
bool StreamNode::streamExist(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->streamExist(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    return (_streams.count(name) > 0);
}
        
std::string StreamNode::streamDescription(
    const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->streamDescription(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_streams.count(name) > 0) {
        return _streams.at(name).comment;
    } else {
        throw std::logic_error(
            "RhIO unknown stream name: " + name);
    }
}
        
void StreamNode::enableStreamingStream(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->enableStreamingStream(tmpName);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_streams.count(name) > 0) {
        _streams.at(name).buffer->_streamWatchers++;
    } else {
        throw std::logic_error(
            "RhIO unknown stream name: " + name);
    }
}
void StreamNode::disableStreamingStream(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->disableStreamingStream(tmpName);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_streams.count(name) > 0) {
        _streams.at(name).buffer->_streamWatchers--;
        if (_streams.at(name).buffer->_streamWatchers < 0) {
            _streams.at(name).buffer->_streamWatchers = 0;
        }
    } else {
        throw std::logic_error(
            "RhIO unknown stream name: " + name);
    }
}
void StreamNode::checkStreamingStream(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->checkStreamingStream(tmpName);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_streams.count(name) > 0) {
        if (_streams.at(name).buffer->_streamWatchers <= 0) {
            _streams.at(name).buffer->_streamWatchers = 1;
        }
    } else {
        throw std::logic_error(
            "RhIO unknown stream name: " + name);
    }
}
        
std::ostream& StreamNode::out(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->out(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_streams.count(name) > 0) {
        return *(_streams.at(name).stream);
    } else {
        throw std::logic_error(
            "RhIO unknown stream name: " + name);
    }
}
        
void StreamNode::newStream(const std::string& name,
    const std::string& comment)
{
    //Forward to subtree
    std::string tmpName;
    StreamNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) {
        child->newStream(tmpName, comment);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_streams.count(name) == 0) {
        _streams[name] = Stream();
        _streams.at(name).comment = comment;
        _streams.at(name).buffer = 
            std::make_shared<StreamBuffer>(
                BaseNode::pwd + separator + name);
        _streams.at(name).stream = 
            std::make_shared<std::ostream>(
            _streams.at(name).buffer.get());
    } else {
        throw std::logic_error(
            "RhIO already register stream name: " + name);
    }
}
        
std::vector<std::string> StreamNode::listStreams() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& c : _streams) {
        list.push_back(c.first);
    }

    return list;
}

}

