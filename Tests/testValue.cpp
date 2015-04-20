#include <iostream>
#include <cassert>
#include "RhIO.hpp"

void printTree(const RhIO::IONode& node, std::string prefix = "")
{
    std::cout << prefix << "--" << node.name() << std::endl;
    for (const auto& param : node.accessValuesBool()) {
        std::cout << prefix << "(bool) " << param.first 
            << "=" << param.second.value << std::endl;
    }
    for (const auto& param : node.accessValuesInt()) {
        std::cout << prefix << "(int) " << param.first 
            << "=" << param.second.value << std::endl;
    }
    for (const auto& param : node.accessValuesFloat()) {
        std::cout << prefix << "(float) " << param.first 
            << "=" << param.second.value << std::endl;
    }
    for (const auto& param : node.accessValuesStr()) {
        std::cout << prefix << "(str) " << param.first 
            << "=" << param.second.value << std::endl;
    }
    for (const auto& child : node.accessChildren()) {
        printTree(child.second, prefix + "  ");
    }
}

int main() 
{
    RhIO::Root.newChild("test");
    RhIO::Root.newChild("test2/pouet");

    assert(RhIO::Root.child("test").accessValuesBool().size() == 0);
    assert(RhIO::Root.child("test").accessValuesInt().size() == 0);
    assert(RhIO::Root.child("test").accessValuesFloat().size() == 0);
    assert(RhIO::Root.child("test").accessValuesStr().size() == 0);

    RhIO::Root.newBool("test/paramBool");
    assert(RhIO::Root.child("test").accessValuesBool().size() == 1);
    assert(RhIO::Root.getValueType("test/paramBool") == RhIO::TypeBool);
    assert(RhIO::Root.getValueType("test/paramBool2") == RhIO::NoValue);
    assert(RhIO::Root.getBool("test/paramBool") == false);
    assert(RhIO::Root.getValueBool("test/paramBool").persisted == false);
    
    RhIO::Root.newInt("test/test3/paramInt")
        ->minimum(-1)
        ->maximum(10);
    assert(RhIO::Root.child("test/test3").accessValuesInt().size() == 1);
    assert(RhIO::Root.child("test/test3").getValueType("paramInt") == RhIO::TypeInt);
    assert(RhIO::Root.getValueType("test/test3/paramInt") == RhIO::TypeInt);
    assert(RhIO::Root.child("test").getValueType("test3/paramInt") == RhIO::TypeInt);
    assert(RhIO::Root.getInt("test/test3/paramInt") == 0);
    assert(RhIO::Root.getValueInt("test/test3/paramInt").persisted == false);
    assert(RhIO::Root.getValueInt("test/test3/paramInt").hasMin == true);
    assert(RhIO::Root.getValueInt("test/test3/paramInt").hasMax == true);
    assert(RhIO::Root.getValueInt("test/test3/paramInt").min == -1);
    assert(RhIO::Root.getValueInt("test/test3/paramInt").max == 10);
    
    RhIO::Root.child("test").newFloat("paramFloat")
        ->comment("this is a test float")
        ->defaultValue(42.0)
        ->isPersisted(true);
    assert(RhIO::Root.child("test").accessValuesFloat().size() == 1);
    assert(RhIO::Root.child("test").accessValuesFloat().count("paramFloat") == 1);
    assert(RhIO::Root.getValueType("test/paramFloat") == RhIO::TypeFloat);
    assert(RhIO::Root.getValueFloat("test/paramFloat").comment == "this is a test float");
    assert(RhIO::Root.getValueFloat("test/paramFloat").value == 42.0);
    assert(RhIO::Root.getFloat("test/paramFloat") == 42.0);
    assert(RhIO::Root.getValueFloat("test/paramFloat").persisted == true);
    assert(RhIO::Root.getValueFloat("test/paramFloat").hasMin == false);
    assert(RhIO::Root.getValueFloat("test/paramFloat").hasMax == false);
    
    RhIO::Root.child("test/test3").newStr("paramStr");
    assert(RhIO::Root.child("test/test3").accessValuesStr().size() == 1);

    assert(RhIO::Root.getInt("test/test3/paramInt") == 0);
    RhIO::Root.setInt("test/test3/paramInt", 4);
    assert(RhIO::Root.getInt("test/test3/paramInt") == 4);
    
    assert(RhIO::Root.getFloat("test/paramFloat") == 42.0);
    RhIO::Root.setFloat("test/paramFloat", 1.0);
    assert(RhIO::Root.getFloat("test/paramFloat") == 1.0);
    
    assert(RhIO::Root.getStr("test/test3/paramStr") == "");
    RhIO::Root.setStr("test/test3/paramStr", "cool!");
    assert(RhIO::Root.getStr("test/test3/paramStr") == "cool!");

    std::cout << "Printing tree:" << std::endl;
    printTree(RhIO::Root); 

    return 0;
}

