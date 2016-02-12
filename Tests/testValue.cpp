#include <iostream>
#include <cassert>
#include "RhIO.hpp"

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
    RhIO::Root.newChild("test");
    RhIO::Root.newChild("test2/pouet");

    assert(RhIO::Root.child("test").listValuesBool().size() == 0);
    assert(RhIO::Root.child("test").listValuesInt().size() == 0);
    assert(RhIO::Root.child("test").listValuesFloat().size() == 0);
    assert(RhIO::Root.child("test").listValuesStr().size() == 0);

    RhIO::Root.newBool("test/paramBool");
    assert(RhIO::Root.child("test").listValuesBool().size() == 1);
    assert(RhIO::Root.getValueType("test/paramBool") == RhIO::TypeBool);
    assert(RhIO::Root.getValueType("test/paramBool2") == RhIO::NoValue);
    assert(RhIO::Root.getBool("test/paramBool") == false);
    assert(RhIO::Root.getValueBool("test/paramBool").persisted == false);
    
    RhIO::Root.newInt("test/test3/paramInt")
        ->minimum(-1)
        ->maximum(10);
    assert(RhIO::Root.child("test/test3").listValuesInt().size() == 1);
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
        ->persisted(true);
    assert(RhIO::Root.child("test").listValuesFloat().size() == 1);
    assert(RhIO::Root.child("test").listValuesFloat()[0] == "paramFloat");
    assert(RhIO::Root.getValueType("test/paramFloat") == RhIO::TypeFloat);
    assert(RhIO::Root.getValueFloat("test/paramFloat").comment == "this is a test float");
    assert(RhIO::Root.getValueFloat("test/paramFloat").value == 42.0);
    assert(RhIO::Root.getFloat("test/paramFloat") == 42.0);
    assert(RhIO::Root.getValueFloat("test/paramFloat").persisted == true);
    assert(RhIO::Root.getValueFloat("test/paramFloat").hasMin == false);
    assert(RhIO::Root.getValueFloat("test/paramFloat").hasMax == false);
    
    RhIO::Root.child("test/test3").newStr("paramStr");
    assert(RhIO::Root.child("test/test3").listValuesStr().size() == 1);

    assert(RhIO::Root.getInt("test/test3/paramInt") == 0);
    RhIO::Root.setInt("test/test3/paramInt", 4);
    assert(RhIO::Root.getInt("test/test3/paramInt") == 4);
    
    assert(RhIO::Root.getInt("test/test3/paramInt") == 4);
    RhIO::Root.setInt("test/test3/paramInt", 20);
    assert(RhIO::Root.getInt("test/test3/paramInt") == 10);
    RhIO::Root.setInt("test/test3/paramInt", -10);
    assert(RhIO::Root.getInt("test/test3/paramInt") == -1);
    
    assert(RhIO::Root.getFloat("test/paramFloat") == 42.0);
    RhIO::Root.setFloat("test/paramFloat", 1.0);
    assert(RhIO::Root.getFloat("test/paramFloat") == 1.0);
    
    assert(RhIO::Root.getStr("test/test3/paramStr") == "");
    RhIO::Root.setStr("test/test3/paramStr", "cool!");
    assert(RhIO::Root.getStr("test/test3/paramStr") == "cool!");

    std::cout << "Printing tree:" << std::endl;
    printTree(RhIO::Root); 

    bool isSet;
    isSet = false;
    RhIO::Root.setBool("test/paramBool", true);
    assert(isSet == false);
    RhIO::Root.setCallbackBool("test/paramBool", [&isSet](bool val) {
        if (val) isSet = true;
    });
    assert(isSet == false);
    RhIO::Root.setBool("test/paramBool", true);
    assert(isSet == true);
    
    isSet = false;
    RhIO::Root.setInt("test/test3/paramInt", 2);
    assert(isSet == false);
    RhIO::Root.setCallbackInt("test/test3/paramInt", [&isSet](int val) {
        if (val == 5) isSet = true;
    });
    assert(isSet == false);
    RhIO::Root.setInt("test/test3/paramInt", 5);
    assert(isSet == true);
    
    isSet = false;
    RhIO::Root.setFloat("test/paramFloat", 2.0);
    assert(isSet == false);
    RhIO::Root.setCallbackFloat("test/paramFloat", [&isSet](double val) {
        if (val == 5.0) isSet = true;
    });
    assert(isSet == false);
    RhIO::Root.setFloat("test/paramFloat", 5.0);
    assert(isSet == true);

    return 0;
}

