#include <iostream>
#include "Shell.h"
#include "RepeatCommand.h"
#include "RemoteCommand.h"
#include <unistd.h>

inline bool inputAvailable()  
{
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);

  return (FD_ISSET(0, &fds));
}

namespace RhIO
{
    std::string RepeatCommand::getName()
    {
        return "repeat";
    }

    std::string RepeatCommand::getDesc()
    {
        return "Repeats a command";
    }

    void RepeatCommand::process(std::vector<std::string> args)
    {
        std::string command = "";
        for (auto part : args) {
            command += part;
            command += " ";
        }

        while (!inputAvailable()) {
            system("clear");
            shell->parse(command);

            std::this_thread::sleep_for(
                std::chrono::milliseconds(100));
        }
    }
}
