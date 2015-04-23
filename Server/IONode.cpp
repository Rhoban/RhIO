#include "IONode.hpp"
#include "Filesystem.hpp"

namespace RhIO {
        
IONode::IONode() :
    ValueNode(std::bind(
        &IONode::forwardChildren, 
        this, std::placeholders::_1, 
        std::placeholders::_2, 
        std::placeholders::_3)),
    _name("ERROR"),
    _pwd("ERROR"),
    _parent(nullptr),
    _children(),
    _mutex()
{
}

IONode::IONode(const std::string& name, IONode* parent) :
    ValueNode(std::bind(
        &IONode::forwardChildren, 
        this, std::placeholders::_1, 
        std::placeholders::_2, 
        std::placeholders::_3)),
    _name(name),
    _pwd(""),
    _parent(parent),
    _children(),
    _mutex()
{
    if (_parent != nullptr) {
        if (_parent->_name != "ROOT") {
            _pwd = _parent->_pwd + separator + name;
        } else {
            _pwd = name;
        }
    }
    BaseNode<ValueNode>::pwd = _pwd;
}
        
IONode::IONode(const IONode& node) :
    ValueNode(std::bind(
        &IONode::forwardChildren, 
        this, std::placeholders::_1, 
        std::placeholders::_2, 
        std::placeholders::_3)),
    _name(node._name),
    _pwd(node._pwd),
    _parent(node._parent),
    _children(node._children),
    _mutex()
{
    BaseNode<ValueNode>::pwd = _pwd;
}
IONode& IONode::operator=(const IONode& node)
{
    ValueNode::operator=(node);
    _name = node._name;
    _pwd = node._pwd;
    _parent = node._parent;
    _children = node._children;
    BaseNode<ValueNode>::forwardFunc = std::bind(
        &IONode::forwardChildren, 
        this, std::placeholders::_1, 
        std::placeholders::_2, 
        std::placeholders::_3);
    BaseNode<ValueNode>::pwd = _pwd;

    return *this;
}
        
const std::string& IONode::name() const
{
    return _name;
}
        
const std::string& IONode::pwd() const
{
    return _pwd;
}

const IONode& IONode::parent() const
{
    if (_parent == nullptr) {
        return *this;
    } else {
        return *_parent;
    }
}
IONode& IONode::parent()
{
    if (_parent == nullptr) {
        return *this;
    } else {
        return *_parent;
    }
}
        
const IONode& IONode::root() const
{
    const IONode* ptOld = this;
    const IONode* pt = _parent;
    while (pt != nullptr) {
        ptOld = pt;
        pt = pt->_parent;
    }

    return *ptOld; 
}
IONode& IONode::root()
{
    IONode* ptOld = this;
    IONode* pt = _parent;
    while (pt != nullptr) {
        ptOld = pt;
        pt = pt->_parent;
    }

    return *ptOld; 
}
        
bool IONode::childExist(const std::string& name) const
{
    try {
        //Forward to subtree
        std::string tmpName;
        const IONode* child = const_cast<IONode*>(this)
            ->forwardChildren(name, tmpName, false);
        if (child != nullptr) return child->childExist(tmpName);
    } catch (const std::logic_error& e) {
        return false;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    return (_children.count(name) > 0);
}

const IONode& IONode::child(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    const IONode* child = const_cast<IONode*>(this)
        ->forwardChildren(name, tmpName, false);
    if (child != nullptr) return child->child(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_children.count(name) == 0) {
        throw std::logic_error("RhIO unknown node name: " + name);
    } else {
        return _children.at(name);
    }
}
IONode& IONode::child(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    IONode* child = forwardChildren(name, tmpName, false);
    if (child != nullptr) return child->child(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_children.count(name) == 0) {
        throw std::logic_error("RhIO unknown node name: " + name);
    } else {
        return _children.at(name);
    }
}
        
void IONode::newChild(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    IONode* child = forwardChildren(name, tmpName, true);
    if (child != nullptr) {
        child->newChild(tmpName);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_children.count(name) == 0) {
        _children[name] = IONode(name, this);
    }
}
        
std::vector<std::string> IONode::listChildren() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& c : _children) {
        list.push_back(c.first);
    }
    
    return list;
}
        
void IONode::save(const std::string& path) const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list = listDirectories(path);

    //Check that all folder is associated with a children
    for (size_t i=0;i<list.size();i++) {
        if (list[i] != "." && list[i] != "..") {
            if (!_children.count(list[i])) {
                throw std::runtime_error(
                    "RhIO unexpected folder in save path: " 
                    + path + ": " + list[i]);
            }
        }
    }

    //Create missing folder associated with children
    for (const auto& c : _children) {
        bool found = false;
        for (size_t j=0;j<list.size();j++) {
            if (c.first == list[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            createDirectory(path, c.first);
        }
    }

    //Save all persisted values
    ValueNode::saveValues(path);

    //Recursive call to children
    for (const auto& c : _children) {
        if (
            path.length() > 0 && 
            path[path.length()-1] != separator
        ) {
            c.second.save(path + separator + c.first);
        } else {
            c.second.save(path + c.first);
        }
    }
}

void IONode::load(const std::string& path)
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list = listDirectories(path);

    //Adding children
    for (size_t i=0;i<list.size();i++) {
        if (
            list[i] != "." && list[i] != ".." && 
            _children.count(list[i]) == 0
        ) {
            _children[list[i]] = IONode(list[i], this);
        }
    }
    
    //Load all persisted values
    ValueNode::loadValues(path);
    
    //Recursive call to children
    for (auto& c : _children) {
        if (
            path.length() > 0 && 
            path[path.length()-1] != separator
        ) {
            c.second.load(path + separator + c.first);
        } else {
            c.second.load(path + c.first);
        }
    }
}
        
IONode* IONode::forwardChildren(
    const std::string& name, std::string& newName,
    bool createBranch)
{
    IONode* pt = this;
    bool isSubNode = false;
    size_t pos = 0;
    while (true) {
        size_t p = name.find_first_of(separator, pos);
        if (p == std::string::npos) {
            if (isSubNode) {
                newName = name.substr(pos);
            }
            break;
        } else {
            isSubNode = true;
            if (p == 0) {
                if (_parent == nullptr) {
                    pos++;
                    continue;
                } else {
                    throw std::logic_error("RhIO invalid name: " + name);
                }
            }
            std::string part = name.substr(pos, p-pos);
            pos = p+1;
            if (createBranch) {
                std::lock_guard<std::mutex> lock(pt->_mutex);
                if (pt->_children.count(part) == 0) {
                    pt->_children[part] = IONode(part, pt);
                }
            } else {
                std::lock_guard<std::mutex> lock(_mutex);
                if (pt->_children.count(part) == 0) {
                    throw std::logic_error(
                        "RhIO unknown node name: " + part);
                }
            }
            std::lock_guard<std::mutex> lock(_mutex);
            pt = &(pt->_children.at(part));
        }
    }

    if (isSubNode) {
        return pt;
    } else {
        return nullptr;
    }
}

}

