#include <map>
#include <iostream>
#include "Shell.h"
#include "PadCommand.h"
#include "joystick/Joystick.h"
#include <json/json.h>

static std::string file_get_contents(std::string path)
{
  std::ifstream ifs(path.c_str());
  std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
  return content;
}

namespace RhIO
{
std::string PadCommand::getName()
{
  return "pad";
}

std::string PadCommand::getDesc()
{
  return "Dump/Use the joystick";
}

void PadCommand::process(std::vector<std::string> args)
{
  Joystick js;

  if (js.open())
  {
    if (args.size() == 0)
    {
      // Mode dump
      Terminal::setColor("white", true);
      std::cout << "Dumping joystick events" << std::endl;
      Terminal::clear();

      while (!shell->hasInput())
      {
        Joystick::JoystickEvent evt;
        if (js.getEvent(&evt))
        {
          if (evt.number != 24 && evt.number != 25 && evt.number != 26)
          {
            if (evt.type == JS_EVENT_AXIS)
            {
              std::cout << "AXIS: " << (int)evt.number << " -> " << evt.getValue() << std::endl;
            }
            else
            {
              std::cout << "BTN: " << (int)evt.number << " -> " << evt.isPressed() << std::endl;
            }
          }
        }
      }
    }
    else
    {
      std::map<int, PadIO> pads = import(args[0]);

      while (!shell->hasInput())
      {
        Joystick::JoystickEvent evt;
        if (js.getEvent(&evt))
        {
          int id = evt.number;
          if (evt.type != JS_EVENT_AXIS && evt.type != JS_EVENT_BUTTON)
          {
            continue;
          }
          if (evt.type == JS_EVENT_AXIS)
          {
            id += 100;
          }
          if (pads.count(id))
          {
            auto& pad = pads[id];
            if (pad.type == PAD_AXIS || pad.type == PAD_BUTTON)
            {
              pad.value = evt.value;
              if (pad.type == PAD_AXIS)
              {
                pad.fvalue = evt.getValue();
                float delta = (pad.max - pad.min) / 2;
                pad.fvalue *= delta;
                pad.fvalue += (pad.min + pad.max) / 2.0;
              }
              update(pad);
            }
            else if (evt.value == 1)
            {
              if (pad.type == PAD_COMMAND)
              {
                shell->parse(pad.command);
              }
              else
              {
                if (pad.type == PAD_TOGGLE)
                {
                  pad.value = !pad.value;
                }
                shell->getFromServer(pad.node);
                pad.fvalue = Node::toNumber(pad.node.value);
                if (pad.type == PAD_INCREMENT)
                {
                  pad.fvalue += pad.step;
                }
                if (pad.type == PAD_DECREMENT)
                {
                  pad.fvalue -= pad.step;
                }
                update(pad);
              }
            }
          }
        }
      }
    }
  }
  else
  {
    throw std::runtime_error(std::string("Can't open the joypad ") + js.getDeviceName());
  }

  js.close();
}

void PadCommand::update(PadIO& pad)
{
  if (pad.type == PAD_AXIS || pad.type == PAD_INCREMENT || pad.type == PAD_DECREMENT)
  {
    shell->setFromNumber(pad.node, pad.fvalue);
  }
  else
  {
    shell->setFromNumber(pad.node, pad.value);
  }
}

std::map<int, PadIO> PadCommand::import(std::string name)
{
  std::map<int, PadIO> pads;

  // Loading file
  char* homedir;
  std::string path;
  if ((homedir = getenv("HOME")) != NULL)
  {
    path = homedir;
    path += "/.rhio/";
    path += name;
  }

  Json::Value value;
  Json::Reader reader;

  if (reader.parse(file_get_contents(path), value))
  {
    if (value.isArray())
    {
      for (unsigned int k = 0; k < value.size(); k++)
      {
        auto entry = value[k];
        if (entry.isObject())
        {
          if (entry.isMember("number") && ((entry.isMember("type") && entry.isMember("param")) || entry.isMember("comma"
                                                                                                                 "nd")))
          {
            PadIO pad;
            pad.value = 0;
            pad.fvalue = 0;
            pad.step = 1;
            pad.id = entry["number"].asInt();
            pad.param = entry["param"].asString();

            if (entry.isMember("command"))
            {
              pad.command = entry["command"].asString();
              pad.type = PAD_COMMAND;
            }
            else
            {
              pad.node = shell->getNodeValue(pad.param);
              if (pad.node.value == NULL)
              {
                std::stringstream ss;
                ss << "Can't find node " << pad.param;
                throw std::runtime_error(ss.str());
              }
              std::string type = entry["type"].asString();
              if (type == "axis")
              {
                pad.type = PAD_AXIS;
                pad.id += 100;
              }
              else if (type == "toggle")
              {
                pad.type = PAD_TOGGLE;
              }
              else if (type == "increment")
              {
                pad.type = PAD_INCREMENT;
              }
              else if (type == "decrement")
              {
                pad.type = PAD_DECREMENT;
              }
              else
              {
                pad.type = PAD_BUTTON;
              }
              if (entry.isMember("range") && entry["range"].isArray() && entry["range"].size() == 2)
              {
                pad.min = entry["range"][0].asFloat();
                pad.max = entry["range"][1].asFloat();
              }
              if (entry.isMember("step") && entry["step"].isNumeric())
              {
                pad.step = entry["step"].asFloat();
              }
            }
            pads[pad.id] = pad;
          }
        }
      }
    }
  }
  else
  {
    std::stringstream ss;
    ss << "Error while loading " << path << std::endl;
    ss << reader.getFormattedErrorMessages();
    throw std::runtime_error(ss.str());
  }

  return pads;
}
}  // namespace RhIO
