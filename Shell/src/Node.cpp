#include <math.h>
#include <iostream>
#include <sstream>
#include "Node.h"

namespace RhIO
{
std::string NodeItem::getPath()
{
  return node->getPath();
}

std::string NodeItem::getName()
{
  auto path = getPath();
  if (path != "")
  {
    path += "/";
  }

  return path + name;
}

NodeValue::NodeValue() : value(NULL), node(NULL)
{
}

NodeValue::NodeValue(Node* node_, ValueBase* value_) : value(value_), node(node_)
{
}

std::string NodeValue::getName()
{
  return node->getPath() + "/" + value->name;
}

std::vector<NodeCommand> Node::getCommands()
{
  return commands;
}

std::vector<NodeStream> Node::getStreams()
{
  return streams;
}

std::vector<NodeFrame> Node::getFrames()
{
  return frames;
}

std::vector<std::string> Node::getChildren()
{
  std::vector<std::string> names;

  for (auto entry : children)
  {
    names.push_back(entry.first);
  }

  return names;
}

Node* Node::getParent()
{
  return parent ? parent : this;
}

Node::Node(ClientReq* client_, std::string path) : parent(NULL), name(""), client(client_)
{
  slashed = path;
  if (path != "")
  {
    slashed += "/";
  }

  // Getting the nodes
#define GET_CHILDREN(type, vect)                                                                                       \
  for (auto name : client->listValues##type(path))                                                                     \
  {                                                                                                                    \
    auto fullName = slashed + name;                                                                                    \
    auto meta = client->metaValue##type(fullName);                                                                     \
    meta.value = client->get##type(fullName);                                                                          \
    meta.name = name;                                                                                                  \
    vect.push_back(meta);                                                                                              \
  }

  GET_CHILDREN(Bool, bools)
  GET_CHILDREN(Float, floats)
  GET_CHILDREN(Int, ints)
  GET_CHILDREN(Str, strings)

  // Commands
  for (auto name : client->listCommands(path))
  {
    NodeCommand command;
    command.node = this;
    command.name = name;
    command.desc = client->commandDescription(slashed + name);
    commands.push_back(command);
  }

  // Streams
  for (auto name : client->listStreams(path))
  {
    NodeStream stream;
    stream.node = this;
    stream.name = name;
    stream.desc = client->streamDescription(slashed + name);
    streams.push_back(stream);
  }

  // Frames
  for (auto name : client->listFrames(path))
  {
    auto meta = client->metaValueFrame(slashed + name);
    NodeFrame frame;
    frame.node = this;
    frame.name = name;
    frame.desc = meta.comment;
    frame.format = meta.format;
    frames.push_back(frame);
  }

  // Getting childrens
  for (auto name : client->listChildren(path))
  {
    children[name] = NULL;
  }
}

std::string Node::getPath()
{
  std::string path = "";
  if (parent != NULL)
  {
    path += parent->getPath();
    if (path != "")
    {
      path += "/";
    }
  }
  path += name;

  return path;
}

Node::~Node()
{
  for (auto entry : children)
  {
    if (entry.second != NULL)
    {
      delete entry.second;
    }
  }
}

Node* Node::getChild(std::string name, bool loadIt)
{
  if (children.count(name))
  {
    if (children[name] == NULL && loadIt)
    {
      children[name] = new Node(client, slashed + name);
      children[name]->name = name;
      children[name]->parent = this;
    }

    return children[name];
  }

  return NULL;
}

NodeValue Node::getNodeValue(std::string name)
{
  auto all = getAll();

  for (auto entry : all)
  {
    if (entry.value->name == name)
    {
      return entry;
    }
  }

  return NodeValue(this, NULL);
}

std::vector<NodeValue> Node::getAll()
{
  std::vector<NodeValue> values;

  for (auto& var : bools)
    values.push_back(NodeValue(this, &var));
  for (auto& var : ints)
    values.push_back(NodeValue(this, &var));
  for (auto& var : floats)
    values.push_back(NodeValue(this, &var));
  for (auto& var : strings)
    values.push_back(NodeValue(this, &var));

  return values;
}

ValueBool* Node::asBool(ValueBase* value)
{
  return dynamic_cast<ValueBool*>(value);
}

ValueInt* Node::asInt(ValueBase* value)
{
  return dynamic_cast<ValueInt*>(value);
}

ValueFloat* Node::asFloat(ValueBase* value)
{
  return dynamic_cast<ValueFloat*>(value);
}

ValueStr* Node::asString(ValueBase* value)
{
  return dynamic_cast<ValueStr*>(value);
}

std::string Node::toString(ValueBase* value)
{
  if (auto val = asBool(value))
  {
    return val->value ? "true" : "false";
  }
  else if (auto val = asInt(value))
  {
    std::stringstream ss;
    ss << val->value;
    return ss.str();
  }
  else if (auto val = asFloat(value))
  {
    std::stringstream ss;
    ss << val->value;
    return ss.str();
  }
  else if (auto val = asString(value))
  {
    return val->value;
  }
  else
  {
    return "?";
  }
}

double Node::toNumber(ValueBase* value)
{
  if (auto val = asBool(value))
  {
    return val->value;
  }
  else if (auto val = asInt(value))
  {
    return val->value;
  }
  else if (auto val = asFloat(value))
  {
    return val->value;
  }

  return 0;
}

std::string Node::getType(ValueBase* value)
{
  if (asBool(value))
  {
    return "bool";
  }
  else if (asInt(value))
  {
    return "int";
  }
  else if (asFloat(value))
  {
    return "float";
  }
  else if (asString(value))
  {
    return "string";
  }
  else
  {
    return "?";
  }
}

std::string Node::persistedToString(ValueBase* value)
{
  if (auto val = asBool(value))
  {
    return val->valuePersisted ? "true" : "false";
  }
  else if (auto val = asInt(value))
  {
    std::stringstream ss;
    ss << val->valuePersisted;
    return ss.str();
  }
  else if (auto val = asFloat(value))
  {
    std::stringstream ss;
    ss << val->valuePersisted;
    return ss.str();
  }
  else if (auto val = asString(value))
  {
    return val->valuePersisted;
  }
  else
  {
    return "?";
  }
}

bool Node::isDiff(ValueBase* value)
{
  if (auto val = asBool(value))
  {
    return val->value != val->valuePersisted;
  }
  else if (auto val = asInt(value))
  {
    return val->value != val->valuePersisted;
  }
  else if (auto val = asFloat(value))
  {
    return fabs(val->value - val->valuePersisted) >= 1e-4;
  }
  else if (auto val = asString(value))
  {
    return val->value != val->valuePersisted;
  }
  else
  {
    return false;
  }
}
}  // namespace RhIO
