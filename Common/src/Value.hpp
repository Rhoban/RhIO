#ifndef RHIO_VALUE_HPP
#define RHIO_VALUE_HPP

#include <iostream>
#include <string>
#include <functional>
#include <chrono>

namespace RhIO
{
/**
 * ValueBase
 *
 * Factorize non type specific
 * field of Value
 */
struct ValueBase
{
  virtual ~ValueBase(){};

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
  int64_t streamWatchers;
};

/**
 * Value
 *
 * Generic value holder
 * for parameters and monitor
 */
template <typename T>
struct Value : public ValueBase
{
  /**
   * Typedef value type
   */
  typedef T Type;

  /**
   * Value value
   */
  T value;

  /**
   * Value min and max bounds
   */
  T min;
  T max;

  /**
   * The value stored in read config file
   * (used to compute values diff)
   */
  T valuePersisted;

  /**
   * Callback called on value update.
   * Take the new value as argument.
   */
  std::function<void(T)> callback;
};

/**
 * Typedef for used value types
 */
typedef Value<bool> ValueBool;
typedef Value<int64_t> ValueInt;
typedef Value<double> ValueFloat;
typedef Value<std::string> ValueStr;

/**
 * Enum for used value type
 */
enum ValueType
{
  NoValue = 0,
  TypeBool = 1,
  TypeInt = 2,
  TypeFloat = 3,
  TypeStr = 4
};

/**
 * Proxy struct used to configure optional
 * value parameters
 */
template <typename T>
struct ValueBuilder final
{
public:
  /**
   * Initialization with builded
   * value reference and flag indicating if
   * the underlying value is brand new or not
   */
  ValueBuilder(Value<T>& val, bool isExisting) : _isExisting(isExisting), _value(val)
  {
    // Default value parameters
    if (!isExisting)
    {
      _value.comment = "";
      _value.hasMin = false;
      _value.hasMax = false;
      _value.value = T();
      _value.valuePersisted = T();
      _value.persisted = false;
      _value.streamWatchers = 0;
      _value.callback = [](T t) { (void)t; };
    }
  }

  /**
   * This callback is called when the value builder is finished (destructed)
   */
  std::function<void(T)> callback;

  /**
   * Optional parameters setters
   */
  ValueBuilder* comment(const std::string& str)
  {
    _value.comment = str;
    return this;
  }
  ValueBuilder* minimum(T val)
  {
    _value.hasMin = true;
    _value.min = val;
    return this;
  }
  ValueBuilder* maximum(T val)
  {
    _value.hasMax = true;
    _value.max = val;
    return this;
  }
  ValueBuilder* defaultValue(T val)
  {
    if (!_isExisting)
    {
      _value.value = val;
      if (callback && !_value.persisted)
      {
        callback(val);
      }
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
   * Internal builded reference instance
   */
  Value<T>& _value;
};

/**
 * Typedef for ValueBuilder
 */
typedef ValueBuilder<bool> ValueBuilderBool;
typedef ValueBuilder<int64_t> ValueBuilderInt;
typedef ValueBuilder<double> ValueBuilderFloat;
typedef ValueBuilder<std::string> ValueBuilderStr;

}  // namespace RhIO

#endif
