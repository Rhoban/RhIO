#include <iostream>
#include <cassert>
#include "RhIO.hpp"

void printTree(const RhIO::IONode& node, std::string prefix = "")
{
    std::cout << prefix << "--" << node.name() << std::endl;
    for (const auto& child : node.listChildren()) {
        printTree(node.child(child), prefix + "  ");
    }
}

int main() 
{
    if (!RhIO::started()) {
        RhIO::start();
    }
    assert(RhIO::started());

    assert(RhIO::Root.name() == "ROOT");
    assert(RhIO::Root.listChildren().size() == 0);
    assert(RhIO::Root.parent().name() == "ROOT");
    assert(RhIO::Root.root().name() == "ROOT");
    assert(RhIO::Root.childExist("test") == false);
    assert(RhIO::Root.childExist("test/test2/pouet") == false);
    assert(RhIO::Root.pwd() == "");
    
    RhIO::Root.newChild("test");
    assert(RhIO::Root.name() == "ROOT");
    assert(RhIO::Root.listChildren().size() == 1);
    assert(RhIO::Root.parent().name() == "ROOT");
    assert(RhIO::Root.root().name() == "ROOT");
    assert(RhIO::Root.childExist("test") == true);
    assert(RhIO::Root.child("test").name() == "test");
    assert(RhIO::Root.child("test").parent().name() == "ROOT");
    assert(RhIO::Root.child("test").root().name() == "ROOT");
    assert(RhIO::Root.child("test").listChildren().size() == 0);
    assert(RhIO::Root.child("test").pwd() == "test");
    
    RhIO::Root.newChild("test2/pouet");
    assert(RhIO::Root.name() == "ROOT");
    assert(RhIO::Root.listChildren().size() == 2);
    assert(RhIO::Root.childExist("test") == true);
    assert(RhIO::Root.childExist("test2") == true);
    assert(RhIO::Root.childExist("test2/pouet") == true);
    assert(RhIO::Root.child("test").name() == "test");
    assert(RhIO::Root.child("test").parent().name() == "ROOT");
    assert(RhIO::Root.child("test").root().name() == "ROOT");
    assert(RhIO::Root.child("test").listChildren().size() == 0);
    
    assert(RhIO::Root.child("test2").name() == "test2");
    assert(RhIO::Root.child("test2").parent().name() == "ROOT");
    assert(RhIO::Root.child("test2").root().name() == "ROOT");
    assert(RhIO::Root.child("test2").listChildren().size() == 1);
    
    assert(RhIO::Root.child("test2/pouet").name() == "pouet");
    assert(RhIO::Root.child("test2").child("pouet").name() == "pouet");
    assert(RhIO::Root.child("test2/pouet").parent().name() == "test2");
    assert(RhIO::Root.child("test2/pouet").root().name() == "ROOT");
    assert(RhIO::Root.child("test2/pouet").listChildren().size() == 0);
    assert(RhIO::Root.child("test2/pouet").pwd() == "test2/pouet");

    std::cout << "Display Tree" << std::endl;
    printTree(RhIO::Root); 

    return 0;
}
