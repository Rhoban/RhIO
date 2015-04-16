#include "IONode.hpp"

namespace RhIO {
        
IONode::IONode() :
    ValueNode(std::bind(
        &IONode::forwardChildren, 
        this, std::placeholders::_1, 
        std::placeholders::_2, 
        std::placeholders::_3)),
    _name("ERROR"),
    _parent(nullptr)
{
}

IONode::IONode(const std::string& name, IONode* parent) :
    ValueNode(std::bind(
        &IONode::forwardChildren, 
        this, std::placeholders::_1, 
        std::placeholders::_2, 
        std::placeholders::_3)),
    _name(name),
    _parent(parent)
{
}
        
const std::string& IONode::name() const
{
    return _name;
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
    //Forward to subtree
    std::string tmpName;
    const IONode* child = const_cast<IONode*>(this)
        ->forwardChildren(name, tmpName, false);
    if (child != nullptr) return child->childExist(tmpName);

    return (_children.count(name) > 0);
}

const IONode& IONode::child(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    const IONode* child = const_cast<IONode*>(this)
        ->forwardChildren(name, tmpName, false);
    if (child != nullptr) return child->child(tmpName);

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

    if (_children.count(name) == 0) {
        _children[name] = IONode(name, this);
    }
}
        
const std::map<std::string, IONode>& IONode::accessChildren() const
{
    return _children;
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
                throw std::logic_error("RhIO invalid name: " + name);
            }
            std::string part = name.substr(pos, p-pos);
            pos = p+1;
            if (pt->_children.count(part) == 0) {
                if (createBranch) {
                    pt->_children[part] = IONode(part, pt);
                } else {
                    throw std::logic_error(
                        "RhIO unknown node name: " + part);
                }
            }
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

