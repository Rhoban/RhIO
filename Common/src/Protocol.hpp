#ifndef RHIO_PROTOCOL_HPP
#define RHIO_PROTOCOL_HPP

#include <vector>
#include <string>

namespace RhIO {

/**
 * Server replier and Server publisher port
 */
extern unsigned int PortServerRep;
extern unsigned int PortServerPub;

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
     * Client
     * Ask for streaming enable and disable 
     * on given absolute value name (for any type).
     * (Streaming watcher is incremented/decremented)
     * Args:
     * String: absolute value name
     */
    MsgEnableStreamingValue,
    MsgDisableStreamingValue,
    /**
     * Client
     * Ask for streaming enable and disable 
     * on given absolute stream name (for any type).
     * (Streaming watcher is incremented/decremented)
     * Args:
     * String: absolute stream name
     */
    MsgEnableStreamingStream,
    MsgDisableStreamingStream,
    /**
     * Client
     * Ask for streaming enable and disable 
     * on given absolute frame name.
     * (Streaming watcher is incremented/decremented)
     * Args:
     * String: absolute frame name
     */
    MsgEnableStreamingFrame,
    MsgDisableStreamingFrame,
    /**
     * Client.
     * Ask for Server persist dump and load for all
     * subtree below given node into given server
     * side path directory
     * Args:
     * String: absolute node name
     * String: configuration path server side
     */
    MsgAskSave,
    MsgAskLoad,
    /**
     * Client.
     * List all registered command relative
     * name at given node absolute name
     * Args:
     * String: absolute node name
     */
    MsgAskCommands,
    /**
     * Client.
     * Ask the description
     * for absolute command name 
     * Args:
     * String: absolute command name
     */
    MsgAskCommandDescription,
    /**
     * Client.
     * Ask for listing all commands
     * alsolute name.
     * Args:
     * None
     */
    MsgAskAllCommands,
    /**
     * Client.
     * Call the given absolute name command
     * with given string arguments list
     * Args:
     * String: absolute command name
     * Int: number of argument
     * String: argument 1
     * String: argument 2
     * ...
     */
    MsgAskCall,
    /**
     * Client.
     * List all registered streams relative
     * name at given node absolute name
     * Args:
     * String: absolute node name
     */
    MsgAskStreams,
    /**
     * Client.
     * Ask given absolute stream name
     * textual comment information
     * Args:
     * String: absolute stream name
     */
    MsgAskDescriptionStream,
    /**
     * Client.
     * List all registered frames relative
     * name at given node absolute name
     * Args:
     * String: absolute node name
     */
    MsgAskFrames,
    /**
     * Client.
     * Ask given absolute frame name
     * meta information
     * Args:
     * String: absolute frame name
     */
    MsgAskMetaFrame,
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
     * Int: number of watcher for streaming
     *
     * Bool: minimum value
     * Bool: maximum value
     * Bool: persisted value
     * or
     * Int: minimum value
     * Int: maximum value
     * Int: persisted value
     * or
     * Float: minimum value
     * Float: maximum value
     * Float: persisted value
     * or
     * Str: minimum value
     * Str: maximum value
     * Str: persisted value
     */
    MsgValMetaBool,
    MsgValMetaInt,
    MsgValMetaFloat,
    MsgValMetaStr,
    /**
     * Server.
     * Return streamed values for type 
     * Bool, Int, Float, Str, Stream or Frame
     * Args:
     * String: value absolute name
     * Int: timestamp
     * Bool: value
     * or
     * Int: value
     * or
     * Float: value
     * or
     * Str: value
     * or
     * Str: value
     * or 
     * Int: size
     * Int: image width
     * Int: image height
     * Data: image raw data
     */
    MsgStreamBool,
    MsgStreamInt,
    MsgStreamFloat,
    MsgStreamStr,
    MsgStreamStream,
    MsgStreamFrame,
    /**
     * Server.
     * Return acknowledge when persist 
     * operation is finished
     */
    MsgPersistOK,
    /**
     * Server.
     * Return the string description
     * of asked command
     * Args:
     * String: the coomand description
     */
    MsgCommandDescription,
    /**
     * Server.
     * Return call result of asked
     * command call
     * Args:
     * String: call result
     */
    MsgCallResult,
    /**
     * Server.
     * Return description for asked
     * stream
     */
    MsgDescriptionStream,
    /**
     * Server.
     * Return meta information 
     * for asked frame
     * Args:
     * String: frame comment
     * Int: frame type
     * Int: number of watcher for streaming
     */
    MsgValMetaFrame,
    /**
     * Server.
     * Acknowledge previous streaming
     * config update
     */
    MsgStreamingOK,
};

}

#endif

