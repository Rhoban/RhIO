#include <iostream>
#include <cassert>
#include "RhIO.hpp"
#include "ClientReq.hpp"

int main() 
{
    RhIO::ClientReq client("tcp://localhost:9999");
    
    std::vector<std::string> list;
    
    list = client.listChildren("ROOT");
    assert(list.size() == 2);
    assert(list[0] == "test");
    assert(list[1] == "test2");
    list = client.listChildren("test2");
    assert(list.size() == 1);
    assert(list[0] == "pouet");

    list = client.listValuesBool("test");
    assert(list.size() == 1);
    assert(list[0] == "paramBool");
    list = client.listValuesBool("test2");
    assert(list.size() == 0);
    list = client.listValuesInt("test/test3");
    assert(list.size() == 1);
    assert(list[0] == "paramInt");
    list = client.listValuesFloat("test");
    assert(list.size() == 1);
    assert(list[0] == "paramFloat");
    list = client.listValuesStr("test/test3");
    assert(list.size() == 1);
    assert(list[0] == "paramStr");

    assert(client.getBool("test/paramBool") == false);
    assert(client.getInt("test/test3/paramInt") == 0);
    assert(client.getFloat("test/paramFloat") == 42.0);
    assert(client.getStr("test/test3/paramStr") == "");
    
    client.setBool("test/paramBool", true);
    assert(client.getBool("test/paramBool") == true);
    client.setInt("test/test3/paramInt", 4);
    assert(client.getInt("test/test3/paramInt") == 4);
    client.setFloat("test/paramFloat", 1.0);
    assert(client.getFloat("test/paramFloat") == 1.0);
    client.setStr("test/test3/paramStr", "cool!");
    assert(client.getStr("test/test3/paramStr") == "cool!");

    return 0;
}

