#ifndef RHIO_VALUE_HPP
#define RHIO_VALUE_HPP

#include <string>
#include <chrono>

namespace RhIO {

/**
 * Value
 *
 * Generic value holder
 * for parameters and monitor
 */
template <typename T>
struct Value final
{
    /**
     * Typedef value type
     */
    typedef T Type;

    /**
     * Value name
     */
    std::string name;

    /**
     * A friendly and helpful comment message
     */
    std::string comment;

    /**
     * Value value
     */
    T value;

    /**
     * Value min and max bounds
     * and flags indicating if bounds are defined
     */
    T min;
    T max;
    bool hasMin;
    bool hasMax;

    /**
     * Last updated value timestamp
     */
    std::chrono::system_clock::time_point timestamp;

    /**
     * If false, the value will not be
     * saved in config files
     */
    bool persisted;
};

/**
 * Typedef for used value types
 */
typedef Value<bool> ValueBool;
typedef Value<long> ValueInt;
typedef Value<double> ValueFloat;
typedef Value<std::string> ValueStr;

/**
 * Enum for used value type
 */
enum ValueType {
    NoValue = 0,
    TypeBool = 1,
    TypeInt = 2,
    TypeFloat = 3,
    TypeStr = 4
};

}

#endif

