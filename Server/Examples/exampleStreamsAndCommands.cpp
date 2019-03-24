#include <iostream>
#include "RhIO.hpp"

int main()
{
  // Like values, streams are associated with a name
  // and stored into tree nodes. They are representing
  // a write-only output string stream (std:ostream)
  // equivalent to standart C++ iostream (cout, cerr).
  // Their main purpose is debug. All streams are
  // received by network remote client in order to be displayed.

  // Like values, they have to be created before use.
  // First argument is a relative new name for the steam
  // and the second is an helpful string descripbing the
  // purpose of the stream.
  RhIO::Root.newStream("path/in/tree/stream1", "description of stream1");
  RhIO::Root.child("path/in/tree").newStream("stream2", "description of stream2");

  // All streams in a node can also be listed by
  // a vector of stream names.
  std::vector<std::string> list1 = RhIO::Root.child("path/in/tree").listStreams();
  for (size_t i = 0; i < list1.size(); i++)
  {
    std::cout << "Stream name: " << list1[i] << std::endl;
  }

  // Finnaly, write to a stream is equivalent as writing
  // into std::cout or std::cerr. Data are sent to client
  // once the stream is flused (for example by std::endl).
  RhIO::Root.out("path/in/tree/stream1") << "This is a debug message" << std::endl;
  RhIO::Root.child("path/in/tree").out("stream2") << "Print test: " << 42 << " " << 3.14 << std::endl;

  // Commands are server side function which can be called
  // by remote client. They are associated with a name and stored
  // inside tree nodes. All arguments are given through a vector
  // of string and the function have also to return a string
  // as return value type.

  // First argument is a new relative name. Second argument
  // is also an helpful description message and the third argument
  // is the function. The function can be a C++11 lambda function
  // or a more classical function pointer with following prototype:
  // std::string function(const std::vector<std::string>&);
  RhIO::Root.newCommand("path/in/tree/command1", "description of command1",
                        [](const std::vector<std::string>& args) -> std::string { return "Hello " + args[0]; });

  // Commands can be listed in the same way as streams and values
  std::vector<std::string> list2 = RhIO::Root.child("path/in/tree").listCommands();
  for (size_t i = 0; i < list2.size(); i++)
  {
    std::cout << "Commands name: " << list2[i] << std::endl;
  }

  // Finally, a command can also be called from server side
  // by providing textual arguments.
  // First argument is the called command name in tree and the second
  // argument is the vector of string arguments.
  std::cout << RhIO::Root.call("path/in/tree/command1", { "World !" }) << std::endl;

  return 0;
}
