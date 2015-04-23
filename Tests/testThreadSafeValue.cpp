#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include "RhIO.hpp"

void printTree(const RhIO::IONode& node, std::string prefix = "")
{
    std::cerr << prefix << "--" << node.name() << std::endl;
    for (const auto& child : node.listChildren()) {
        printTree(node.child(child), prefix + "  ");
    }
}

void function1()
{
    RhIO::Root.newBool("test/bool1");
    RhIO::Root.newInt("test/int1");
    RhIO::Root.newFloat("test/float1");
    RhIO::Root.newStr("test/str1");
    RhIO::Root.newBool("test/bool2");
    RhIO::Root.newInt("test/int2");
    RhIO::Root.newFloat("test/float2");
    RhIO::Root.newStr("test/str2");
    RhIO::Root.newBool("test/bool3");
    RhIO::Root.newInt("test/int3");
    RhIO::Root.newFloat("test/float3");
    RhIO::Root.newStr("test/str3");
    RhIO::Root.newBool("test/bool4");
    RhIO::Root.newInt("test/int4");
    RhIO::Root.newFloat("test/float4");
    RhIO::Root.newStr("test/str4");
}

void function2()
{
    RhIO::Root.newBool("test/bool4");
    RhIO::Root.newInt("test/int4");
    RhIO::Root.newFloat("test/float4");
    RhIO::Root.newStr("test/str4");
    RhIO::Root.newBool("test/bool3");
    RhIO::Root.newInt("test/int3");
    RhIO::Root.newFloat("test/float3");
    RhIO::Root.newStr("test/str3");
    RhIO::Root.newBool("test/bool2");
    RhIO::Root.newInt("test/int2");
    RhIO::Root.newFloat("test/float2");
    RhIO::Root.newStr("test/str2");
    RhIO::Root.newBool("test/bool1");
    RhIO::Root.newInt("test/int1");
    RhIO::Root.newFloat("test/float1");
    RhIO::Root.newStr("test/str1");
}

void function3()
{
    std::vector<std::string> list;
    
    std::vector<std::string> tmp;
    tmp = RhIO::Root.child("test").listValuesBool();
    for (const auto n : tmp) {
        list.push_back("1: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesInt();
    for (const auto n : tmp) {
        list.push_back("1: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesFloat();
    for (const auto n : tmp) {
        list.push_back("1: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesStr();
    for (const auto n : tmp) {
        list.push_back("1: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesBool();
    for (const auto n : tmp) {
        list.push_back("2: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesInt();
    for (const auto n : tmp) {
        list.push_back("2: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesFloat();
    for (const auto n : tmp) {
        list.push_back("2: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesStr();
    for (const auto n : tmp) {
        list.push_back("2: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesBool();
    for (const auto n : tmp) {
        list.push_back("3: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesInt();
    for (const auto n : tmp) {
        list.push_back("3: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesFloat();
    for (const auto n : tmp) {
        list.push_back("3: " + n);
    }
    tmp = RhIO::Root.child("test").listValuesStr();
    for (const auto n : tmp) {
        list.push_back("3: " + n);
    }
    
    for (size_t i=0;i<list.size();i++) {
        std::cerr << "- " << list[i] << std::endl;
    }
}

void function4()
{
    std::vector<std::string> list;
    if (RhIO::Root.getValueType("test/bool1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool1")));
    }
    if (RhIO::Root.getValueType("test/int1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int1")));
    }
    if (RhIO::Root.getValueType("test/float1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float1")));
    }
    if (RhIO::Root.getValueType("test/str1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str1")));
    }
    if (RhIO::Root.getValueType("test/bool2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool2")));
    }
    if (RhIO::Root.getValueType("test/int2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int2")));
    }
    if (RhIO::Root.getValueType("test/float2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float2")));
    }
    if (RhIO::Root.getValueType("test/str2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str2")));
    }
    if (RhIO::Root.getValueType("test/bool3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool3")));
    }
    if (RhIO::Root.getValueType("test/int3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int3")));
    }
    if (RhIO::Root.getValueType("test/float3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float3")));
    }
    if (RhIO::Root.getValueType("test/str3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str3")));
    }
    if (RhIO::Root.getValueType("test/bool1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool1")));
    }
    if (RhIO::Root.getValueType("test/int1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int1")));
    }
    if (RhIO::Root.getValueType("test/float1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float1")));
    }
    if (RhIO::Root.getValueType("test/str1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str1")));
    }
    if (RhIO::Root.getValueType("test/bool2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool2")));
    }
    if (RhIO::Root.getValueType("test/int2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int2")));
    }
    if (RhIO::Root.getValueType("test/float2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float2")));
    }
    if (RhIO::Root.getValueType("test/str2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str2")));
    }
    if (RhIO::Root.getValueType("test/bool3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool3")));
    }
    if (RhIO::Root.getValueType("test/int3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int3")));
    }
    if (RhIO::Root.getValueType("test/float3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float3")));
    }
    if (RhIO::Root.getValueType("test/str3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str3")));
    }
    if (RhIO::Root.getValueType("test/bool1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool1")));
    }
    if (RhIO::Root.getValueType("test/int1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int1")));
    }
    if (RhIO::Root.getValueType("test/float1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float1")));
    }
    if (RhIO::Root.getValueType("test/str1") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str1")));
    }
    if (RhIO::Root.getValueType("test/bool2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool2")));
    }
    if (RhIO::Root.getValueType("test/int2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int2")));
    }
    if (RhIO::Root.getValueType("test/float2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float2")));
    }
    if (RhIO::Root.getValueType("test/str2") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str2")));
    }
    if (RhIO::Root.getValueType("test/bool3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/bool3")));
    }
    if (RhIO::Root.getValueType("test/int3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/int3")));
    }
    if (RhIO::Root.getValueType("test/float3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/float3")));
    }
    if (RhIO::Root.getValueType("test/str3") == RhIO::TypeBool) {
        list.push_back(std::to_string(RhIO::Root.getBool("test/str3")));
    }
    
    for (size_t i=0;i<list.size();i++) {
        std::cerr << "- " << list[i] << std::endl;
    }
}

int main()
{
    RhIO::Root.newChild("test");

    std::thread t1(function1);
    std::thread t2(function2);
    std::thread t3(function3);
    std::thread t4(function4);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    
    std::cerr << "Display Tree" << std::endl;
    printTree(RhIO::Root); 

    return 0;
}

