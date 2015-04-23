#include <stdexcept>
#include "CommandNode.hpp"
#include "RhIO.hpp"

namespace RhIO {

CommandNode& CommandNode::operator=(const CommandNode& node)
{
    _commands = node._commands;
    _descriptions = node._descriptions;

    return *this;
}
        
bool CommandNode::commandExist(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    CommandNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->commandExist(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    return (_commands.count(name) > 0);
}
        
std::string CommandNode::call(const std::string& name, 
    const std::vector<std::string>& arguments)
{
    //Forward to subtree
    std::string tmpName;
    CommandNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->call(tmpName, arguments);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_commands.count(name) > 0) {
        return _commands.at(name)(arguments);
    } else {
        throw std::logic_error(
            "RhIO unknown command name: " + name);
    }
}

std::string CommandNode::commandDescription(
    const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    CommandNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->commandDescription(tmpName);
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_descriptions.count(name) > 0) {
        return _descriptions.at(name);
    } else {
        throw std::logic_error(
            "RhIO unknown command name: " + name);
    }
}

void CommandNode::newCommand(const std::string& name, 
    const std::string& comment,
    CommandFunc func)
{
    //Forward to subtree
    std::string tmpName;
    CommandNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) {
        child->newCommand(tmpName, comment, func);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_commands.count(name) == 0) {
        _commands[name] = func;
        _descriptions[name] = comment;
    } else {
        throw std::logic_error(
            "RhIO already register command name: " + name);
    }
}
        
std::vector<std::string> CommandNode::listCommands() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& c : _commands) {
        list.push_back(c.first);
    }

    return list;
}

}

