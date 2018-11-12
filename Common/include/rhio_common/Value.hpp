#ifndef RHIO_VALUE_HPP
#define RHIO_VALUE_HPP

#include <string>
#include <functional>
#include <atomic>
#include <chrono>

namespace RhIO {

/**
 * ValueBase
 *
 * Factorize non type specific 
 * field of Value
 */
struct ValueBase
{
    virtual ~ValueBase() {};

    /**
     * Value name
     */
    std::string name;

    /**
     * A friendly and helpful comment message
     */
    std::string comment;

    /**
     * Flags indicating min & max presence
     */
    bool hasMin;
    bool hasMax;

    /**
     * Last updated value timestamp
     */
    std::chrono::steady_clock::time_point timestamp;

    /**
     * If false, the value will not be
     * saved in config files
     */
    bool persisted;

    /**
     * The number of registered watcher
     * Streaming is enabled while at least
     * one watcher is registered
     */
    std::atomic<int64_t> streamWatchers;

    /**
     * Default constructor
     */
    ValueBase() :
        name(""),
        comment(""),
        hasMin(false),
        hasMax(false),
        timestamp(),
        persisted(false),
        streamWatchers(0)
    {
    }

    /**
     * Copy constructor
     */
    ValueBase(const ValueBase& v) :
        name(v.name),
        comment(v.comment),
        hasMin(v.hasMin),
        hasMax(v.hasMax),
        timestamp(v.timestamp),
        persisted(v.persisted),
        streamWatchers(v.streamWatchers.load())
    {
    }

    /**
     * Assignment operation
     */
    ValueBase& operator=(const ValueBase& v)
    {
        if (this != &v) {
            name = v.name;
            comment = v.comment;
            hasMin = v.hasMin;
            hasMax = v.hasMax;
            timestamp = v.timestamp;
            persisted = v.persisted;
            streamWatchers.store(v.streamWatchers.load());
        }

        return *this;
    }
};

/**
 * Value
 *
 * Generic value holder
 * for parameters and monitor.
 * Type for actual value and meta
 * information are provided.
 */
template <typename TypeVal, typename TypeRaw>
struct Value : public ValueBase
{
    /**
     * Typedef value type
     */
    typedef TypeRaw Type;

    /**
     * Value value
     */
    TypeVal value;

    /**
     * Value min and max bounds
     */
    TypeRaw min;
    TypeRaw max;

    /**
     * The value stored in read config file
     * (used to compute values diff)
     */
    TypeRaw valuePersisted;

    /**
     * Callback called on value update.
     * Take the new value as argument.
     */
    std::function<void(TypeRaw)> callback;
};

/**
 * Partial template specialization for Value
 * for atomic type value.
 * Add copy and assignment because atomic is non copyable.
 */
template <typename TypeVal, typename TypeRaw>
struct Value<std::atomic<TypeVal>, TypeRaw> : public ValueBase
{
    /**
     * Typedef value type
     */
    typedef TypeRaw Type;

    /**
     * Value value
     */
    std::atomic<TypeVal> value;

    /**
     * Value min and max bounds
     */
    TypeRaw min;
    TypeRaw max;

    /**
     * The value stored in read config file
     * (used to compute values diff)
     */
    TypeRaw valuePersisted;

    /**
     * Callback called on value update.
     * Take the new value as argument.
     */
    std::function<void(TypeRaw)> callback;

    /**
     * Default constructor
     */
    Value() :
        ValueBase(),
        value(),
        min(),
        max(),
        valuePersisted(),
        callback()
    {
    }

    /**
     * Copy constructor
     */
    Value(const Value<std::atomic<TypeVal>, TypeRaw>& v) :
        ValueBase(v),
        value(v.value.load()),
        min(v.min),
        max(v.max),
        valuePersisted(v.valuePersisted),
        callback(v.callback)
    {
    }

    /**
     * Assignment operator
     */
    Value<std::atomic<TypeVal>, TypeRaw>& operator=(const Value<std::atomic<TypeVal>, TypeRaw>& v)
    {
        if (this != &v) {
            ValueBase::operator=(v);
            value.store(v.value.load());
            min = v.min;
            max = v.max;
            valuePersisted = v.valuePersisted;
            callback = v.callback;
        }
        return *this;
    }
};

/**
 * Typedef for used value types
 */
typedef Value<std::atomic<int64_t>, bool> ValueBool;
typedef Value<std::atomic<int64_t>, int64_t> ValueInt;
typedef Value<std::atomic<double>, double> ValueFloat;
typedef Value<std::string, std::string> ValueStr;

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

/**
 * Proxy struct used to configure optional
 * value parameters.
 * Type for actual value and meta
 * information are provided.
 */
template <typename TypeVal, typename TypeRaw>
struct ValueBuilder final
{
    public:

        /**
         * Initialization with built
         * value reference and flag indicating if
         * the underlying value is brand new or not
         */
        ValueBuilder(Value<TypeVal, TypeRaw>& val, bool isExisting) :
            _isExisting(isExisting),
            _value(val)
        {
            //Default value parameters
            if (!isExisting) {
                _value.comment = "";
                _value.hasMin = false;
                _value.hasMax = false;
                _value.value = TypeRaw();
                _value.valuePersisted = TypeRaw();
                _value.persisted = false;
                _value.streamWatchers = 0;
                _value.callback = [](TypeRaw t){(void)t;};
            }
        }

        /**
         * Optional parameters setters
         */
        ValueBuilder* comment(const std::string& str)
        {
            _value.comment = str;
            return this;
        }
        ValueBuilder* minimum(TypeRaw val)
        {
            _value.hasMin = true;
            _value.min = val;
            return this;
        }
        ValueBuilder* maximum(TypeRaw val)
        {
            _value.hasMax = true;
            _value.max = val;
            return this;
        }
        ValueBuilder* defaultValue(TypeRaw val)
        {
            if (!_isExisting) {
                _value.value = val;
                _value.valuePersisted = val;
            }
            return this;
        }
        ValueBuilder* persisted(bool flag)
        {
            _value.persisted = flag;
            return this;
        }

    private:

        /**
         * True if we are dealing with already
         * existing value. Thus no update.
         */
        bool _isExisting;

        /**
         * Internal built reference instance
         */
        Value<TypeVal, TypeRaw>& _value;
};

/**
 * Typedef for ValueBuilder
 */
typedef ValueBuilder<std::atomic<int64_t>, bool> ValueBuilderBool;
typedef ValueBuilder<std::atomic<int64_t>, int64_t> ValueBuilderInt;
typedef ValueBuilder<std::atomic<double>, double> ValueBuilderFloat;
typedef ValueBuilder<std::string, std::string> ValueBuilderStr;

}

#endif

