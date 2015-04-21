#ifndef RHIO_PROTOCOL_HPP
#define RHIO_PROTOCOL_HPP

#include <vector>
#include <string>

namespace RhIO {

/**
 * Protocol message type
 */
enum MsgType : uint8_t {
    /**
     * Client.
     * Ask for listing children Nodes.
     * Args:
     * String: absolute node name to inspect
     */
    MsgAskChildren,
    /**
     * Client.
     * Ask for listing values of type Bool,
     * Int, Float or Str at given Node name.
     * Args:
     * String: absolute node name
     */
    MsgAskValuesBool,
    MsgAskValuesInt,
    MsgAskValuesFloat,
    MsgAskValuesStr,
    /**
     * Client.
     * Ask for values of type Bool, Int,
     * Float, Str
     * Args:
     * String: absolute value name
     */
    MsgGetBool,
    MsgGetInt,
    MsgGetFloat,
    MsgGetStr,
    /**
     * Client.
     * Ask for values update for type
     * Bool, Int, Float and Str
     * Args:
     * String: absolute value name to update
     * Bool : new value
     * or
     * Int : new value
     * or
     * Float : new value
     * or
     * Str : new value
     */
    MsgSetBool,
    MsgSetInt,
    MsgSetFloat,
    MsgSetStr,
    /**
     * Client.
     * Ask for value meta data for type
     * Bool, Int, Float and Str
     * Args:
     * String: absolute value name to update
     */
    MsgAskMetaBool,
    MsgAskMetaInt,
    MsgAskMetaFloat,
    MsgAskMetaStr,
    /**
     * Server.
     * An error has occured.
     * Args:
     * String: error message.
     */
    MsgError,
    /**
     * Server.
     * Return all names of asked children 
     * or values (Bool, Int, Float or Str) of asked Node.
     * Args:
     * Int: number of values
     * String: name 1
     * String: name 2
     * ...
     */
    MsgListNames,
    /**
     * Server.
     * Return the value of asked value
     * for type Bool, Int, Float, Str
     * Args:
     * Bool: value
     * or
     * Int: value
     * or
     * Float: value
     * or
     * Str: value
     */
    MsgValBool,
    MsgValInt,
    MsgValFloat,
    MsgValStr,
    /**
     * Server.
     * Acknowledge the requested set
     * No arguments
     */
    MsgSetOk,
    /**
     * Server.
     * Return all asked value meta information
     * for type Bool, Int, Float and Str
     * Args:
     * String: value comment
     * Bool: has minimum
     * Bool: has maximum
     * Bool: is persisted
     *
     * Bool: minimum value
     * Bool: maximum value
     * or
     * Int: minimum value
     * Int: maximum value
     * or
     * Float: minimum value
     * Float: maximum value
     * or
     * Str: minimum value
     * Str: maximum value
     */
    MsgValMetaBool,
    MsgValMetaInt,
    MsgValMetaFloat,
    MsgValMetaStr,
};

}

#endif

