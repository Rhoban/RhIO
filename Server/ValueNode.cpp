#include <stdexcept>
#include <chrono>
#include "ValueNode.hpp"
#include "ServerPub.hpp"
#include "RhIO.hpp"

namespace RhIO {
        
ValueNode& ValueNode::operator=(const ValueNode& node)
{
    _valuesBool = node._valuesBool;
    _valuesInt = node._valuesInt;
    _valuesFloat = node._valuesFloat;
    _valuesStr = node._valuesStr;

    return *this;
}
        
ValueType ValueNode::getValueType(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getValueType(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) > 0) {
        return TypeBool;
    } else if (_valuesInt.count(name) > 0) {
        return TypeInt;
    } else if (_valuesFloat.count(name) > 0) {
        return TypeFloat;
    } else if (_valuesStr.count(name) > 0) {
        return TypeStr;
    } else {
        return NoValue;
    }
}

bool ValueNode::getBool(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getBool(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Bool name: " + name);
    } else {
        return _valuesBool.at(name).value;
    }
}
long ValueNode::getInt(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getInt(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown Int value name: " + name);
    } else {
        return _valuesInt.at(name).value;
    }
}
double ValueNode::getFloat(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getFloat(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesFloat.count(name) == 0) {
        throw std::logic_error("RhIO unknown Float value name: " + name);
    } else {
        return _valuesFloat.at(name).value;
    }
}
const std::string& ValueNode::getStr(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getStr(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown Str value name: " + name);
    } else {
        return _valuesStr.at(name).value;
    }
}

void ValueNode::setBool(const std::string& name, bool val,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setBool(tmpName, val, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Bool name: " + name);
    } else {
        _valuesBool[name].value = val;
        _valuesBool[name].timestamp = timestamp;
        ServerStream->publishBool(BaseNode::pwd + separator + name, val,
            std::chrono::duration_cast<std::chrono::milliseconds>
                (timestamp.time_since_epoch()).count());
    }
}
void ValueNode::setInt(const std::string& name, long val,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setInt(tmpName, val, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Int name: " + name);
    } else {
        _valuesInt[name].value = val;
        _valuesInt[name].timestamp = timestamp;
        ServerStream->publishInt(BaseNode::pwd + separator + name, val, 
            std::chrono::duration_cast<std::chrono::milliseconds>
                (timestamp.time_since_epoch()).count());
    }
}
void ValueNode::setFloat(const std::string& name, double val,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setFloat(tmpName, val, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesFloat.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Float name: " + name);
    } else {
        _valuesFloat[name].value = val;
        _valuesFloat[name].timestamp = timestamp;
        ServerStream->publishFloat(BaseNode::pwd + separator + name, val,
            std::chrono::duration_cast<std::chrono::milliseconds>
                (timestamp.time_since_epoch()).count());
    }
}
void ValueNode::setStr(const std::string& name, const std::string& val,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setStr(tmpName, val, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Str name: " + name);
    } else {
        _valuesStr[name].value = val;
        _valuesStr[name].timestamp = timestamp;
        ServerStream->publishStr(BaseNode::pwd + separator + name, val,
            std::chrono::duration_cast<std::chrono::milliseconds>
                (timestamp.time_since_epoch()).count());
    }
}

std::unique_ptr<ValueBuilderBool> ValueNode::newBool(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) return child->newBool(tmpName);

    if (getValueType(name) != NoValue) {
        //The value is already registered
        if (getValueType(name) != TypeBool) {
            //Error if type conflic
            throw std::logic_error(
                "RhIO value already known with other type: " + name);
        } else {
            //No conflic
            return std::unique_ptr<ValueBuilderBool>(
                new ValueBuilderBool(_valuesBool[name], true));
        }
    } else {
        //Creating a really new value
        std::lock_guard<std::mutex> lock(_mutex);
        _valuesBool[name] = ValueBool();
        _valuesBool[name].name = name;
        return std::unique_ptr<ValueBuilderBool>(
            new ValueBuilderBool(_valuesBool[name], false));
    }
}
std::unique_ptr<ValueBuilderInt> ValueNode::newInt(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) return child->newInt(tmpName);

    if (getValueType(name) != NoValue) {
        //The value is already registered
        if (getValueType(name) != TypeInt) {
            //Error if type conflic
            throw std::logic_error(
                "RhIO value already known with other type: " + name);
        } else {
            //No conflic
            return std::unique_ptr<ValueBuilderInt>(
                new ValueBuilderInt(_valuesInt[name], true));
        }
    } else {
        //Creating a really new value
        std::lock_guard<std::mutex> lock(_mutex);
        _valuesInt[name] = ValueInt();
        _valuesInt[name].name = name;
        return std::unique_ptr<ValueBuilderInt>(
            new ValueBuilderInt(_valuesInt[name], false));
    }
}
std::unique_ptr<ValueBuilderFloat> ValueNode::newFloat(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) return child->newFloat(tmpName);

    if (getValueType(name) != NoValue) {
        //The value is already registered
        if (getValueType(name) != TypeFloat) {
            //Error if type conflic
            throw std::logic_error(
                "RhIO value already known with other type: " + name);
        } else {
            //No conflic
            return std::unique_ptr<ValueBuilderFloat>(
                new ValueBuilderFloat(_valuesFloat[name], true));
        }
    } else {
        //Creating a really new value
        std::lock_guard<std::mutex> lock(_mutex);
        _valuesFloat[name] = ValueFloat();
        _valuesFloat[name].name = name;
        return std::unique_ptr<ValueBuilderFloat>(
            new ValueBuilderFloat(_valuesFloat[name], false));
    }
}
std::unique_ptr<ValueBuilderStr> ValueNode::newStr(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, true);
    if (child != nullptr) return child->newStr(tmpName);

    if (getValueType(name) != NoValue) {
        //The value is already registered
        if (getValueType(name) != TypeStr) {
            //Error if type conflic
            throw std::logic_error(
                "RhIO value already known with other type: " + name);
        } else {
            //No conflic
            return std::unique_ptr<ValueBuilderStr>(
                new ValueBuilderStr(_valuesStr[name], true));
        }
    } else {
        //Creating a really new value
        std::lock_guard<std::mutex> lock(_mutex);
        _valuesStr[name] = ValueStr();
        _valuesStr[name].name = name;
        return std::unique_ptr<ValueBuilderStr>(
            new ValueBuilderStr(_valuesStr[name], false));
    }
}

const ValueBool& ValueNode::getValueBool(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getValueBool(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Bool name: " + name);
    } else {
        return _valuesBool.at(name);
    }
}
const ValueInt& ValueNode::getValueInt(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getValueInt(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Int name: " + name);
    } else {
        return _valuesInt.at(name);
    }
}
const ValueFloat& ValueNode::getValueFloat(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getValueFloat(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesFloat.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Float name: " + name);
    } else {
        return _valuesFloat.at(name);
    }
}
const ValueStr& ValueNode::getValueStr(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getValueStr(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Str name: " + name);
    } else {
        return _valuesStr.at(name);
    }
}

std::vector<std::string> ValueNode::listValuesBool() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& v : _valuesBool) {
        list.push_back(v.first);
    }

    return list;
}
std::vector<std::string> ValueNode::listValuesInt() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& v : _valuesInt) {
        list.push_back(v.first);
    }

    return list;
}
std::vector<std::string> ValueNode::listValuesFloat() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& v : _valuesFloat) {
        list.push_back(v.first);
    }

    return list;
}
std::vector<std::string> ValueNode::listValuesStr() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<std::string> list;
    for (const auto& v : _valuesStr) {
        list.push_back(v.first);
    }

    return list;
}

}

