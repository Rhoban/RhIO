#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>
#include <vector>
#include "RhIO.hpp"

void printTree(const RhIO::IONode& node, std::string prefix = "")
{
    std::cout << prefix << "--" << node.name() << std::endl;
    for (const auto& child : node.listChildren()) {
        printTree(node.child(child), prefix + "  ");
    }
}

void function1()
{
    RhIO::Root.newChild("l1");
    RhIO::Root.newChild("l2");
    RhIO::Root.newChild("l3");
    RhIO::Root.newChild("l4");
    RhIO::Root.newChild("l5");
    RhIO::Root.newChild("l6");
    RhIO::Root.newChild("l7");
    RhIO::Root.newChild("l8");
    RhIO::Root.newChild("l9");
}

void function2()
{
    RhIO::Root.newChild("l9/ll1");
    RhIO::Root.newChild("l8/ll1");
    RhIO::Root.newChild("l7/ll1");
    RhIO::Root.newChild("l6/ll1");
    RhIO::Root.newChild("l5/ll1");
    RhIO::Root.newChild("l4/ll1");
    RhIO::Root.newChild("l3/ll1");
    RhIO::Root.newChild("l2/ll1");
    RhIO::Root.newChild("l1/ll1");
}

void function3()
{
    RhIO::Root.newChild("l1/ll1/lll1");
    RhIO::Root.newChild("l2/ll1/lll1");
    RhIO::Root.newChild("l3/ll1/lll1");
    RhIO::Root.newChild("l4/ll1/lll1");
    RhIO::Root.newChild("l5/ll1/lll1");
    RhIO::Root.newChild("l6/ll1/lll1");
    RhIO::Root.newChild("l7/ll1/lll1");
    RhIO::Root.newChild("l8/ll1/lll1");
    RhIO::Root.newChild("l9/ll1/lll1");
}

void function4()
{
    std::vector<std::string> list;

    std::vector<std::string> tmp;
    tmp = RhIO::Root.listChildren();
    for (const auto n : tmp) {
        list.push_back("1: " + n);
    }
    tmp = RhIO::Root.listChildren();
    for (const auto n : tmp) {
        list.push_back("2: " + n);
    }
    tmp = RhIO::Root.listChildren();
    for (const auto n : tmp) {
        list.push_back("3: " + n);
    }
    
    for (size_t i=0;i<list.size();i++) {
        std::cout << "- " << list[i] << std::endl;
    }
}

void function5()
{
    std::vector<std::string> list;
    if (RhIO::Root.childExist("l1/ll1")) {
            list.push_back(RhIO::Root.child("l1/ll1").name());
    }
    if (RhIO::Root.childExist("l2/ll1")) {
            list.push_back(RhIO::Root.child("l2/ll1").name());
    }
    if (RhIO::Root.childExist("l3/ll1")) {
            list.push_back(RhIO::Root.child("l3/ll1").name());
    }
    if (RhIO::Root.childExist("l4/ll1")) {
            list.push_back(RhIO::Root.child("l4/ll1").name());
    }
    if (RhIO::Root.childExist("l5/ll1")) {
            list.push_back(RhIO::Root.child("l5/ll1").name());
    }
    if (RhIO::Root.childExist("l6/ll1")) {
            list.push_back(RhIO::Root.child("l6/ll1").name());
    }
    if (RhIO::Root.childExist("l7/ll1")) {
            list.push_back(RhIO::Root.child("l7/ll1").name());
    }
    for (size_t i=0;i<list.size();i++) {
        std::cout << "- " << list[i] << std::endl;
    }
}

int main()
{
    if (!RhIO::started()) {
        RhIO::start();
    }
    assert(RhIO::started());

    std::thread t1(function1);
    std::thread t2(function2);
    std::thread t3(function3);
    std::thread t4(function4);
    std::thread t5(function5);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    
    std::cout << "Display Tree" << std::endl;
    printTree(RhIO::Root); 

    return 0;
}

