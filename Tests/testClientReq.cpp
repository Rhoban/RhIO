#include <iostream>
#include <cassert>
#include "RhIO.hpp"
#include "ClientReq.hpp"

int main() 
{
    RhIO::ClientReq client(
        std::string("tcp://localhost:") + RhIO::ServerRepPort);
    
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

    RhIO::ValueBool valBool = client.metaValueBool("test/paramBool");
    assert(valBool.comment == "");
    assert(valBool.hasMin == false);
    assert(valBool.hasMax == false);
    assert(valBool.persisted == false);
    
    RhIO::ValueInt valInt = client.metaValueInt("test/test3/paramInt");
    assert(valInt.comment == "");
    assert(valInt.hasMin == true);
    assert(valInt.hasMax == true);
    assert(valInt.persisted == false);
    assert(valInt.min == -1);
    assert(valInt.max == 10);
    
    RhIO::ValueFloat valFloat = client.metaValueFloat("test/paramFloat");
    assert(valFloat.comment == "this is a test float");
    assert(valFloat.hasMin == false);
    assert(valFloat.hasMax == false);
    assert(valFloat.persisted == true);
    
    RhIO::ValueStr valStr = client.metaValueStr("test/test3/paramStr");
    assert(valStr.comment == "");
    assert(valStr.hasMin == false);
    assert(valStr.hasMax == false);
    assert(valStr.persisted == false);

    return 0;
}

