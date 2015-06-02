#include <iostream>
#include <cassert>
#include "RhIO.hpp"
#include "Filesystem.hpp"

void printTree(const RhIO::IONode& node, std::string prefix = "")
{
    std::cout << prefix << "--" << node.name() << std::endl;
    for (const auto& param : node.listValuesBool()) {
        std::cout << prefix << "(bool) " << param << std::endl;
    }
    for (const auto& param : node.listValuesInt()) {
        std::cout << prefix << "(int) " << param << std::endl;
    }
    for (const auto& param : node.listValuesFloat()) {
        std::cout << prefix << "(float) " << param << std::endl;
    }
    for (const auto& param : node.listValuesStr()) {
        std::cout << prefix << "(str) " << param << std::endl;
    }
    for (const auto& child : node.listChildren()) {
        printTree(node.child(child), prefix + "  ");
    }
}

int main() 
{
    RhIO::Root.newChild("test2");
    RhIO::Root.load("/tmp/testRhIO");

    assert(RhIO::Root.getValueType("paramBool") == RhIO::TypeBool);
    assert(RhIO::Root.getValueType("test/paramInt") == RhIO::TypeInt);
    assert(RhIO::Root.getValueType("test/paramFloat") == RhIO::TypeFloat);
    assert(RhIO::Root.getValueType("test2/test3/paramStr") == RhIO::TypeStr);

    assert(RhIO::Root.getBool("paramBool") == true);
    assert(RhIO::Root.getInt("test/paramInt") == 42);
    assert(RhIO::Root.getFloat("test/paramFloat") == 3.14);
    assert(RhIO::Root.getStr("test2/test3/paramStr") == "off");
    
    assert(RhIO::Root.getValueBool("paramBool").name == "paramBool");
    assert(RhIO::Root.getValueInt("test/paramInt").name == "paramInt");
    assert(RhIO::Root.getValueFloat("test/paramFloat").name == "paramFloat");
    assert(RhIO::Root.getValueStr("test2/test3/paramStr").name == "paramStr");
    
    assert(RhIO::Root.getValueBool("paramBool").persisted == false);
    assert(RhIO::Root.getValueInt("test/paramInt").persisted == false);
    assert(RhIO::Root.getValueFloat("test/paramFloat").persisted == false);
    assert(RhIO::Root.getValueStr("test2/test3/paramStr").persisted == false);

    assert(RhIO::Root.getValueBool("paramBool").comment == "bool parameter");
    assert(RhIO::Root.getValueInt("test/paramInt").comment == "");
    assert(RhIO::Root.getValueFloat("test/paramFloat").comment == "float parameter");
    assert(RhIO::Root.getValueStr("test2/test3/paramStr").comment == "str parameter");

    assert(RhIO::Root.getValueBool("paramBool").hasMin == false);
    assert(RhIO::Root.getValueBool("paramBool").hasMax == false);
    assert(RhIO::Root.getValueInt("test/paramInt").hasMin == false);
    assert(RhIO::Root.getValueInt("test/paramInt").hasMax == false);
    assert(RhIO::Root.getValueFloat("test/paramFloat").hasMin == false);
    assert(RhIO::Root.getValueFloat("test/paramFloat").hasMax == false);
    assert(RhIO::Root.getValueStr("test2/test3/paramStr").hasMin == false);
    assert(RhIO::Root.getValueStr("test2/test3/paramStr").hasMax == false);
    
    RhIO::Root.setInt("test/paramInt", 0);
    RhIO::Root.setFloat("test/paramFloat", 0.0);
    assert(RhIO::Root.getValueBool("paramBool").valuePersisted == true);
    assert(RhIO::Root.getValueInt("test/paramInt").valuePersisted == 42);
    assert(RhIO::Root.getValueFloat("test/paramFloat").valuePersisted == 3.14);
    assert(RhIO::Root.getValueStr("test2/test3/paramStr").valuePersisted == "off");
    
    std::cout << "Printing tree:" << std::endl;
    printTree(RhIO::Root); 
    
    return 0;
}

