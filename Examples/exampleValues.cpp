#include <iostream>
#include "RhIO.hpp"

int main()
{
    //On creation, optionnal meta information 
    //can be added to values by using the ValueBuilder
    //returned by newBool, newInt, newFloat and newStr
    RhIO::Root.newInt("path/paramInt")
        //Set the value non temporary.
        //When save is called, this value will be writen
        //in config files
        //Default is false.
        ->persisted(true)
        //Optional meta are chained.
        //A description text message describing 
        //the value use.
        //Default is empty string.
        ->comment("this is an kind and helpful description")
        //Set a default value at initialization.
        //Override by manual set or config files value.
        //Default is false, 0 or empty string.
        ->defaultValue(42)
        //Set optional minimum value
        //Default is no minimum defined
        ->minimum(-2)
        //Set optional maximum value
        //Default is no minimum defined
        ->maximum(100);

    //Direct access to the structure
    //with value, name and meta information
    //Since this call return a reference, this is
    //not thread safe
    std::cout << "paramInt name: " 
        << RhIO::Root.getValueInt("path/paramInt").name << std::endl;
    std::cout << "paramInt value: " 
        << RhIO::Root.getValueInt("path/paramInt").value << std::endl;
    std::cout << "paramInt is persisted: " 
        << RhIO::Root.getValueInt("path/paramInt").persisted << std::endl;
    std::cout << "paramInt has minimum: " 
        << RhIO::Root.getValueInt("path/paramInt").hasMin << std::endl;
    std::cout << "paramInt minimum value: " 
        << RhIO::Root.getValueInt("path/paramInt").min << std::endl;
    std::cout << "paramInt has maximum: " 
        << RhIO::Root.getValueInt("path/paramInt").hasMax << std::endl;
    std::cout << "paramInt minimum value: " 
        << RhIO::Root.getValueInt("path/paramInt").max << std::endl;

    //When a value is set, an internal timestamp is update.
    //If the real value update has occured before the call, a timestamp
    //(std::chrono::time_point) can be given.
    RhIO::Root.setInt("path/paramInt", 42,
        std::chrono::steady_clock::now());
    
    //The entire tree hierarchy and all persisted values
    //(persisted flag set to true) can be saved into a directory 
    //with save() call. A path to a folder is given as argument.
    //If the directiry or the path to the directory does not exist,
    //needed folders are created.
    RhIO::Root.save("/tmp/testRhIOExample");

    //Each Node can also save individually its subtree 
    //into given path in same way as Root.
    RhIO::Root.child("path").save("/tmp/testRhIOExample/path");
    
    //Load() operation is the dual of save(). The given directory
    //path is read and the tree and value are created or updated from
    //config file.
    RhIO::Root.load("/tmp/testRhIOExample");
    
    //Same restricted on a node
    RhIO::Root.child("path").load("/tmp/testRhIOExample/path");

    return 0;
}

