#include <iostream>
#include "RhIO.hpp"

int main()
{
    //Start networking server
    if (!RhIO::started()) {
        RhIO::start();
    }

    //The global main instance RhIO::IONode Root is defined
    //on startup and contains shared tree and all values,
    //commands and stream.
    //ZeroMQ Rep and Pub server are also start in their
    //own thread.

    //Values are created for each type with
    //newBool, newInt, newFloat, newStr 
    //and providing a name
    RhIO::Root.newInt("firstValueInt");

    //Values can then be update with
    //setBool, setInt, setFloat and setStr
    RhIO::Root.setInt("firstValueInt", 42);

    //Values can then be accessed with
    //getBool, getInt, getFloat and getStr
    std::cout << "firstValueInt: " 
        << RhIO::Root.getInt("firstValueInt") << std::endl;

    //We can check if a value has been defined
    //by requesting its type which can be either
    //TypeBool, TypeInt, TypeFloat, TypeStr or NoValue.
    if (RhIO::Root.getValueType("firstValueInt") == RhIO::TypeInt) {
        std::cout << "firstValueInt is an Int" << std::endl;
    } else if (RhIO::Root.getValueType("firstValueInt") == RhIO::NoValue) {
        std::cout << "firstValueInt is not yet registered" << std::endl;
    }

    //A value can also be define with an 
    //absolute path in the tree
    //Sub tree nodes are created if they
    //do not exist
    RhIO::Root.newBool("path/in/tree/secondValueBool");

    //A specific tree node can be accessed or store
    //directly through a IONode pointer and 
    //child() method
    RhIO::IONode* node = &(RhIO::Root.child("path/in"));

    //Value can then be retrieve with relative path
    std::cout << "secondValueFloat: "
        << node->getBool("tree/secondValueBool") << std::endl;

    //All children of a node can be retrieve with
    //listChildren(). A vector of string of relative node
    //name is returned.
    std::vector<std::string> list1 = RhIO::Root.listChildren();
    for (size_t i=0;i<list1.size();i++) {
        std::cout << "Child name: " << list1[i] << std::endl;
    }

    //For values, all relative value names can also be retrieved
    //in a vector of string for each types listValuesBool(), 
    //listValuesInt(), listValuesFloat() and listValuesStr()
    std::vector<std::string> list2 = node->child("tree").listValuesBool();
    for (size_t i=0;i<list2.size();i++) {
        std::cout << "Values Bool name: " << list2[i] << std::endl;
    }

    return 0;
}

