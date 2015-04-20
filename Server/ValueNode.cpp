#include <stdexcept>
#include "ValueNode.hpp"

namespace RhIO {
        
ValueType ValueNode::getValueType(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getValueType(tmpName);

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

    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown Str value name: " + name);
    } else {
        return _valuesStr.at(name).value;
    }
}

void ValueNode::setBool(const std::string& name, bool val)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setBool(tmpName, val);
        return;
    }

    if (_valuesBool.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Bool name: " + name);
    } else {
        _valuesBool[name].value = val;
    }
}
void ValueNode::setInt(const std::string& name, long val)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setInt(tmpName, val);
        return;
    }

    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Int name: " + name);
    } else {
        _valuesInt[name].value = val;
    }
}
void ValueNode::setFloat(const std::string& name, double val)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setFloat(tmpName, val);
        return;
    }

    if (_valuesFloat.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Float name: " + name);
    } else {
        _valuesFloat[name].value = val;
    }
}
void ValueNode::setStr(const std::string& name, const std::string& val)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setStr(tmpName, val);
        return;
    }

    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Str name: " + name);
    } else {
        _valuesStr[name].value = val;
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

    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Str name: " + name);
    } else {
        return _valuesStr.at(name);
    }
}

const std::map<std::string, ValueBool>& ValueNode::accessValuesBool() const
{
    return _valuesBool;
}
const std::map<std::string, ValueInt>& ValueNode::accessValuesInt() const
{
    return _valuesInt;
}
const std::map<std::string, ValueFloat>& ValueNode::accessValuesFloat() const
{
    return _valuesFloat;
}
const std::map<std::string, ValueStr>& ValueNode::accessValuesStr() const
{
    return _valuesStr;
}

}

