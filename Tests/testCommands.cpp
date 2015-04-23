#include <iostream>
#include <cassert>
#include "RhIO.hpp"

int main()
{
    RhIO::Root.newChild("test");
    RhIO::Root.newChild("test2/pouet");

    assert(RhIO::Root.commandExist("test") == false);
    assert(RhIO::Root.commandExist("test/command1") == false);
    assert(RhIO::Root.commandExist("test2/pouet/command2") == false);
    assert(RhIO::Root.child("test").listCommands().size() == 0);

    RhIO::Root.newCommand("test/command1", 
        "command1", 
        [](const std::vector<std::string>& args) -> std::string
        {
            std::cout << args[0] << std::endl;
            return "OK";
        });
    RhIO::Root.newCommand("test2/pouet/command2", 
        "command2", 
        [](const std::vector<std::string>& args) -> std::string
        {
            std::cout << args[0] << std::endl;
            return "KO";
        });
    
    assert(RhIO::Root.commandExist("test") == false);
    assert(RhIO::Root.commandExist("test/command1") == true);
    assert(RhIO::Root.commandExist("test2/pouet/command2") == true);
    assert(RhIO::Root.commandDescription("test/command1") == "command1");
    assert(RhIO::Root.commandDescription("test2/pouet/command2") == "command2");
    assert(RhIO::Root.child("test").listCommands().size() == 1);
    assert(RhIO::Root.child("test").listCommands()[0] == "command1");

    assert(RhIO::Root.call("test/command1", {"test1"}) == "OK");
    assert(RhIO::Root.call("test2/pouet/command2", {"test2"}) == "KO");

    return 0;
}

