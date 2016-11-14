#include <stdexcept>
#include <chrono>
#include <fstream>
#include "Server/ValueNode.hpp"
#include "Server/ServerPub.hpp"
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
    try {
        //Forward to subtree
        std::string tmpName;
        ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
        if (child != nullptr) return child->getValueType(tmpName);
    } catch (const std::logic_error& e) {
        return NoValue;
    }

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
        throw std::logic_error("RhIO unknown value Bool name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        return _valuesBool.at(name).value;
    }
}
int64_t ValueNode::getInt(const std::string& name) const
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->getInt(tmpName);

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown Int value name: '" + name + "' in '"
            + BaseNode::pwd + "'");
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
        throw std::logic_error("RhIO unknown Float value name: '" + name + "' in '"
            + BaseNode::pwd + "'");
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
        throw std::logic_error("RhIO unknown Str value name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        return _valuesStr.at(name).value;
    }
}

void ValueNode::setBool(const std::string& name, bool val,
    bool noCallblack,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setBool(tmpName, val, noCallblack, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Bool name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        //Bound to min/max
        if (
            _valuesBool.at(name).hasMin && 
            val < _valuesBool.at(name).min
        ) {
            val = _valuesBool.at(name).min;
        }
        if (
            _valuesBool.at(name).hasMax && 
            val > _valuesBool.at(name).max
        ) {
            val = _valuesBool.at(name).max;
        }
        //Update value
        _valuesBool[name].value = val;
        _valuesBool[name].timestamp = timestamp;
        //Call callback
        if (!noCallblack) {
            _valuesBool[name].callback(val);
        }
        //Publish value
        if (_valuesBool.at(name).streamWatchers > 0) {
            ServerStream->publishBool(BaseNode::pwd + separator + name, val,
                std::chrono::duration_cast<std::chrono::milliseconds>
                    (timestamp.time_since_epoch()).count());
        }
    }
}
void ValueNode::setInt(const std::string& name, int64_t val,
    bool noCallblack,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setInt(tmpName, val, noCallblack, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Int name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        //Bound to min/max
        if (
            _valuesInt.at(name).hasMin && 
            val < _valuesInt.at(name).min
        ) {
            val = _valuesInt.at(name).min;
        }
        if (
            _valuesInt.at(name).hasMax && 
            val > _valuesInt.at(name).max
        ) {
            val = _valuesInt.at(name).max;
        }
        //Update value
        _valuesInt[name].value = val;
        _valuesInt[name].timestamp = timestamp;
        //Call callback
        if (!noCallblack) {
            _valuesInt[name].callback(val);
        }
        //Publish value
        if (_valuesInt.at(name).streamWatchers > 0) {
            ServerStream->publishInt(BaseNode::pwd + separator + name, val, 
                std::chrono::duration_cast<std::chrono::milliseconds>
                    (timestamp.time_since_epoch()).count());
        }
    }
}
void ValueNode::setFloat(const std::string& name, double val,
    bool noCallblack,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setFloat(tmpName, val, noCallblack, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesFloat.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Float name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        //Bound to min/max
        if (
            _valuesFloat.at(name).hasMin && 
            val < _valuesFloat.at(name).min
        ) {
            val = _valuesFloat.at(name).min;
        }
        if (
            _valuesFloat.at(name).hasMax && 
            val > _valuesFloat.at(name).max
        ) {
            val = _valuesFloat.at(name).max;
        }
        //Update value
        _valuesFloat[name].value = val;
        _valuesFloat[name].timestamp = timestamp;
        //Call callback
        if (!noCallblack) {
            _valuesFloat[name].callback(val);
        }
        //Publish value
        if (_valuesFloat.at(name).streamWatchers > 0) {
            ServerStream->publishFloat(BaseNode::pwd + separator + name, val,
                std::chrono::duration_cast<std::chrono::milliseconds>
                    (timestamp.time_since_epoch()).count());
        }
    }
}
void ValueNode::setStr(const std::string& name, const std::string& val,
    bool noCallblack,
    std::chrono::steady_clock::time_point timestamp)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->setStr(tmpName, val, noCallblack, timestamp);
        return;
    }

    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Str name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        //Update value
        _valuesStr[name].value = val;
        _valuesStr[name].timestamp = timestamp;
        //Bound to min/max
        if (
            _valuesStr.at(name).hasMin && 
            val < _valuesStr.at(name).min
        ) {
            _valuesStr[name].value = _valuesStr.at(name).min;
        }
        if (
            _valuesStr.at(name).hasMax && 
            val > _valuesStr.at(name).max
        ) {
            _valuesStr[name].value = _valuesStr.at(name).max;
        }
        //Call callback
        if (!noCallblack) {
            _valuesStr[name].callback(_valuesStr[name].value);
        }
        //Publish value
        if (_valuesStr.at(name).streamWatchers > 0) {
            ServerStream->publishStr(BaseNode::pwd + separator + name, val,
                std::chrono::duration_cast<std::chrono::milliseconds>
                    (timestamp.time_since_epoch()).count());
        }
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
                "RhIO value already known with other type: '" + name + "' in '"
                + BaseNode::pwd + "'");
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
                "RhIO value already known with other type: '" + name + "' in '"
                + BaseNode::pwd + "'");
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
                "RhIO value already known with other type: '" + name + "' in '"
                + BaseNode::pwd + "'");
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
                "RhIO value already known with other type: '" + name + "' in '"
                + BaseNode::pwd + "'");
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
        
void ValueNode::setCallbackBool(const std::string& name,
    std::function<void(bool)> func)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->setCallbackBool(tmpName, func);

    //Assign new callback
    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Bool name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        _valuesBool.at(name).callback = func;
    }
}
void ValueNode::setCallbackInt(const std::string& name,
    std::function<void(int64_t)> func)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->setCallbackInt(tmpName, func);

    //Assign new callback
    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesInt.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Int name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        _valuesInt.at(name).callback = func;
    }
}
void ValueNode::setCallbackFloat(const std::string& name,
    std::function<void(double)> func)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->setCallbackFloat(tmpName, func);

    //Assign new callback
    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesFloat.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Float name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        _valuesFloat.at(name).callback = func;
    }
}
void ValueNode::setCallbackStr(const std::string& name,
    std::function<void(std::string)> func)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) return child->setCallbackStr(tmpName, func);

    //Assign new callback
    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesStr.count(name) == 0) {
        throw std::logic_error("RhIO unknown value Str name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        _valuesStr.at(name).callback = func;
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
        throw std::logic_error("RhIO unknown value Bool name: '" + name + "' in '"
            + BaseNode::pwd + "'");
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
        throw std::logic_error("RhIO unknown value Int name: '" + name + "' in '"
            + BaseNode::pwd + "'");
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
        throw std::logic_error("RhIO unknown value Float name: '" + name + "' in '"
            + BaseNode::pwd + "'");
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
        throw std::logic_error("RhIO unknown value Str name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    } else {
        return _valuesStr.at(name);
    }
}
        
void ValueNode::enableStreamingValue(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->enableStreamingValue(tmpName);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) > 0) {
        _valuesBool.at(name).streamWatchers++;
    } else if (_valuesInt.count(name) > 0) {
        _valuesInt.at(name).streamWatchers++;
    } else if (_valuesFloat.count(name) > 0) {
        _valuesFloat.at(name).streamWatchers++;
    } else if (_valuesStr.count(name) > 0) {
        _valuesStr.at(name).streamWatchers++;
    } else {
        throw std::logic_error(
            "RhIO unknown value name: '" + name + "' in '"
            + BaseNode::pwd + "'");
    }
}
void ValueNode::disableStreamingValue(const std::string& name)
{
    //Forward to subtree
    std::string tmpName;
    ValueNode* child = BaseNode::forwardFunc(name, tmpName, false);
    if (child != nullptr) {
        child->disableStreamingValue(tmpName);
        return;
    }
    
    std::lock_guard<std::mutex> lock(_mutex);
    if (_valuesBool.count(name) > 0) {
        _valuesBool.at(name).streamWatchers--;
        if (_valuesBool.at(name).streamWatchers < 0) {
            _valuesBool.at(name).streamWatchers = 0;
        }
    } else if (_valuesInt.count(name) > 0) {
        _valuesInt.at(name).streamWatchers--;
        if (_valuesInt.at(name).streamWatchers < 0) {
            _valuesInt.at(name).streamWatchers = 0;
        }
    } else if (_valuesFloat.count(name) > 0) {
        _valuesFloat.at(name).streamWatchers--;
        if (_valuesFloat.at(name).streamWatchers < 0) {
            _valuesFloat.at(name).streamWatchers = 0;
        }
    } else if (_valuesStr.count(name) > 0) {
        _valuesStr.at(name).streamWatchers--;
        if (_valuesStr.at(name).streamWatchers < 0) {
            _valuesStr.at(name).streamWatchers = 0;
        }
    } else {
        throw std::logic_error(
            "RhIO unknown value name: '" + name + "' in '"
            + BaseNode::pwd + "'");
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
        
void ValueNode::saveValues(const std::string& path)
{
    std::lock_guard<std::mutex> lock(_mutex);

    //Check that some values have to be saved
    bool hasValues = false;
    for (const auto& v : _valuesBool) {
        if (v.second.persisted) hasValues = true;
    }
    for (const auto& v : _valuesInt) {
        if (v.second.persisted) hasValues = true;
    }
    for (const auto& v : _valuesFloat) {
        if (v.second.persisted) hasValues = true;
    }
    for (const auto& v : _valuesStr) {
        if (v.second.persisted) hasValues = true;
    }
    if (!hasValues) {
        return;
    }

    //Open values.conf file
    std::ofstream file;
    if (
        path.length() > 0 && 
        path[path.length()-1] != separator
    ) {
        file.open(path + separator + "values.conf");
    } else {
        file.open(path + "values.conf");
    }
    if (!file.is_open()) {
        throw std::runtime_error(
            "RhIO unable to write values file: " + path);
    }

    //Write Bool values
    for (auto& v : _valuesBool) {
        if (v.second.persisted) {
            file << "[bool] " << v.second.name 
                << ".value = " << v.second.value << std::endl;
            file << "[bool] " << v.second.name 
                << ".comment = " << v.second.comment << std::endl;
            //Update persisted value
            v.second.valuePersisted = v.second.value;
        }
    }
    //Write Int values
    for (auto& v : _valuesInt) {
        if (v.second.persisted) {
            file << "[int] " << v.second.name 
                << ".value = " << v.second.value << std::endl;
            file << "[int] " << v.second.name 
                << ".comment = " << v.second.comment << std::endl;
            //Update persisted value
            v.second.valuePersisted = v.second.value;
        }
    }
    //Write Float values
    for (auto& v : _valuesFloat) {
        if (v.second.persisted) {
            file << "[float] " << v.second.name 
                << ".value = " << v.second.value << std::endl;
            file << "[float] " << v.second.name 
                << ".comment = " << v.second.comment << std::endl;
            //Update persisted value
            v.second.valuePersisted = v.second.value;
        }
    }
    //Write Str values
    for (auto& v : _valuesStr) {
        if (v.second.persisted) {
            file << "[str] " << v.second.name 
                << ".value = " << v.second.value << std::endl;
            file << "[str] " << v.second.name 
                << ".comment = " << v.second.comment << std::endl;
            //Update persisted value
            v.second.valuePersisted = v.second.value;
        }
    }

    file.close();
}
        
void ValueNode::loadValues(const std::string& path)
{
    //Open values.conf file
    std::ifstream file;
    if (
        path.length() > 0 && 
        path[path.length()-1] != separator
    ) {
        file.open(path + separator + "values.conf");
    } else {
        file.open(path + "values.conf");
    }
    if (!file.is_open()) {
        return;
    }

    auto error = [&path](const std::string& line) {
        throw std::runtime_error(
            "RhIO invalid formated values file: " 
            + path + ": " + line);
    };

    //For each lines
    while (file.good()) {
        std::string line;
        std::getline(file, line);
        if (line.size() == 0) continue;
        if (line[0] != '[') error(line);
        //Extract type
        size_t pos = 0;
        size_t pos2 = line.find_first_of("]", pos);
        if (pos2 == std::string::npos) error(line);
        if (pos2 - pos == 0) error(line);
        pos2++;
        std::string type = line.substr(pos, pos2 - pos);
        //Skip space
        pos = pos2 + 1;
        pos = line.find_first_not_of(" ", pos);
        if (pos == std::string::npos) error(line);
        //Extract name
        pos2 = line.find_first_of(".", pos);
        if (pos2 == std::string::npos) error(line);
        if (pos2 - pos == 0) error(line);
        std::string name = line.substr(pos, pos2 - pos);
        //Extract field
        pos = pos2 + 1;
        pos2 = line.find_first_of(" ", pos);
        if (pos2 == std::string::npos) error(line);
        if (pos2 - pos == 0) error(line);
        std::string field = line.substr(pos, pos2 - pos);
        //Extact value
        pos = pos2 + 1;
        pos = line.find_first_of("=", pos);
        if (pos == std::string::npos) error(line);
        pos++;
        pos = line.find_first_not_of(" ", pos);
        std::string value;
        if (pos == std::string::npos) {
            value = "";
        } else {
            value = line.substr(pos);
        }
        
        //Assign value, comment, min and max
        if (type == "[bool]") {
            //Bool type
            if (_valuesBool.count(name) == 0) {
                _valuesBool[name] = ValueBool();
                _valuesBool[name].name = name;
                ValueBuilderBool(_valuesBool[name], false);
            }
            if (field == "value") {
                _valuesBool.at(name).value = std::stoi(value);
                _valuesBool.at(name).valuePersisted = std::stoi(value);
            } else if (field == "comment") {
                _valuesBool.at(name).comment = value;
            } else {
                error(line);
            }
        } else if (type == "[int]") {
            //Int type
            if (_valuesInt.count(name) == 0) {
                _valuesInt[name] = ValueInt();
                _valuesInt[name].name = name;
                ValueBuilderInt(_valuesInt[name], false);
            }
            if (field == "value") {
                _valuesInt.at(name).value = std::stol(value);
                _valuesInt.at(name).valuePersisted = std::stol(value);
            } else if (field == "comment") {
                _valuesInt.at(name).comment = value;
            } else {
                error(line);
            }
        } else if (type == "[float]") {
            //Float type
            if (_valuesFloat.count(name) == 0) {
                _valuesFloat[name] = ValueFloat();
                _valuesFloat[name].name = name;
                ValueBuilderFloat(_valuesFloat[name], false);
            }
            if (field == "value") {
                _valuesFloat.at(name).value = std::stod(value);
                _valuesFloat.at(name).valuePersisted = std::stod(value);
            } else if (field == "comment") {
                _valuesFloat.at(name).comment = value;
            } else {
                error(line);
            }
        } else if (type == "[str]") {
            //Str type
            if (_valuesStr.count(name) == 0) {
                _valuesStr[name] = ValueStr();
                _valuesStr[name].name = name;
                ValueBuilderStr(_valuesStr[name], false);
            }
            if (field == "value") {
                _valuesStr.at(name).value = value;
                _valuesStr.at(name).valuePersisted = value;
            } else if (field == "comment") {
                _valuesStr.at(name).comment = value;
            } else {
                error(line);
            }
        } else {
            error(line);
        }
    }
    
    file.close();
}

}

