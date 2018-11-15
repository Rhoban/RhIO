#include <stdexcept>
#include <chrono>
#include "rhio_server/FrameNode.hpp"
#include "rhio_server/ServerPub.hpp"
#include "RhIO.hpp"

namespace RhIO {

FrameNode& FrameNode::operator=(const FrameNode& node)
{
    _frames = node._frames;

    return *this;
}

bool FrameNode::frameExist(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->frameExist(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    return (_frames.count(name) > 0);
}

bool FrameNode::frameIsStreaming(
    const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->frameIsStreaming(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) > 0) {
        return (_frames.at(name).countWatchers > 0);
    } else {
        throw std::logic_error(
            "RhIO unknown frame name: " + name);
    }
}

const Frame& FrameNode::getFrame(
    const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getFrame(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) > 0) {
        return _frames.at(name);
    } else {
        throw std::logic_error(
            "RhIO unknown frame name: " + name);
    }
}

void FrameNode::framePush(const std::string& name, 
    size_t width, size_t height,
    unsigned char* data, size_t size, 
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->framePush(tmpName, width, height, data, size);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) > 0) {
        if (size != 3*width*height) {
            throw std::logic_error(
                "RhIO frame size seems wrong: " + name);
        }
        if (_frames.at(name).countWatchers > 0) {
            ServerStream->publishFrame(BaseNode::pwd + separator + name, 
                width, height, 
                data, size,
                std::chrono::duration_cast<std::chrono::milliseconds>
                (timestamp.time_since_epoch()).count());
        }
    } else {
        throw std::logic_error(
            "RhIO unknown frame name: " + name);
    }
}

void FrameNode::enableStreamingFrame(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->enableStreamingFrame(tmpName);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) > 0) {
        _frames.at(name).countWatchers++;
    } else {
        throw std::logic_error(
            "RhIO unknown frame name: " + name);
    }
}
void FrameNode::disableStreamingFrame(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->disableStreamingFrame(tmpName);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) > 0) {
        _frames.at(name).countWatchers--;
        if (_frames.at(name).countWatchers < 0) {
            _frames.at(name).countWatchers = 0;
        }
    } else {
        throw std::logic_error(
            "RhIO unknown frame name: " + name);
    }
}
void FrameNode::checkStreamingFrame(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->checkStreamingFrame(tmpName);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) > 0) {
        if (_frames.at(name).countWatchers <= 0) {
            _frames.at(name).countWatchers = 1;
        }
    } else {
        throw std::logic_error(
            "RhIO unknown frame name: " + name);
    }
}

void FrameNode::newFrame(const std::string& name,
    const std::string& comment, 
    FrameFormat format)
{
    //Forward to subtree
    std::string tmpName;
    FrameNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) {
        child->newFrame(tmpName, comment, format);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_frames.count(name) == 0) {
        _frames[name] = Frame();
        _frames.at(name).name = name;
        _frames.at(name).comment = comment;
        _frames.at(name).format = format;
        _frames.at(name).countWatchers = 0;
    } else {
        throw std::logic_error(
            "RhIO already register frame name: '" + BaseNode::pwd + "/" + name + "'");
    }
}
        
std::vector<std::string> FrameNode::listFrames() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& c : _frames) {
        list.push_back(c.first);
    }

    return list;
}

}

