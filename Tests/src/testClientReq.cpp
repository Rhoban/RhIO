#include <iostream>
#include <cassert>
#include "RhIO.hpp"
#include "RhIOClient.hpp"

int main() 
{
    RhIO::ClientReq client(
        std::string("tcp://localhost:") + std::to_string(RhIO::ServersPortBase));
    
    std::vector<std::string> list;
    
    list = client.listChildren("ROOT");
    assert(list.size() == 2);
    assert(list[0] == "test");
    assert(list[1] == "test2");
    list = client.listChildren("/test2");
    assert(list.size() == 1);
    assert(list[0] == "pouet");
    
    try {
        list = client.listChildren("test/nonode");
        assert(0);
    } catch (const std::runtime_error& e) {
        assert(1);
    }

    list = client.listValuesBool("test");
    assert(list.size() == 1);
    assert(list[0] == "paramBool");
    list = client.listValuesBool("test2");
    assert(list.size() == 0);
    list = client.listValuesInt("/test/test3");
    assert(list.size() == 1);
    assert(list[0] == "paramInt");
    list = client.listValuesFloat("test");
    assert(list.size() == 1);
    assert(list[0] == "paramFloat");
    list = client.listValuesStr("test/test3");
    assert(list.size() == 1);
    assert(list[0] == "paramStr");
    
    try {
        list = client.listValuesBool("test//nonode");
        assert(0);
    } catch (const std::runtime_error& e) {
        assert(1);
    }

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
    assert(valBool.valuePersisted == false);
    
    RhIO::ValueInt valInt = client.metaValueInt("test/test3/paramInt");
    assert(valInt.comment == "");
    assert(valInt.hasMin == true);
    assert(valInt.hasMax == true);
    assert(valInt.persisted == false);
    assert(valInt.min == -1);
    assert(valInt.max == 10);
    assert(valInt.valuePersisted == 0);
    
    RhIO::ValueFloat valFloat = client.metaValueFloat("test/paramFloat");
    assert(valFloat.comment == "this is a test float");
    assert(valFloat.hasMin == false);
    assert(valFloat.hasMax == false);
    assert(valFloat.persisted == true);
    assert(valFloat.valuePersisted == 42.0);
    
    RhIO::ValueStr valStr = client.metaValueStr("test/test3/paramStr");
    assert(valStr.comment == "");
    assert(valStr.hasMin == false);
    assert(valStr.hasMax == false);
    assert(valStr.persisted == false);
    assert(valStr.valuePersisted == "");

    assert(client.listCommands("/").size() == 0);
    assert(client.listCommands("test").size() == 1);
    assert(client.listCommands("test")[0] == "command1");
    assert(client.commandDescription("test/command1") == "command1");
    assert(client.call("test/command1", {"test1"}) == "OK test1");
    
    assert(client.listStreams("/").size() == 0);
    assert(client.listStreams("test").size() == 1);
    assert(client.listStreams("test")[0] == "stream1");
    assert(client.streamDescription("test/stream1") == "stream1");
    
    assert(client.listFrames("/").size() == 0);
    assert(client.listFrames("test").size() == 2);
    assert(client.listFrames("test")[0] == "frame1");
    assert(client.listFrames("test")[1] == "frame2");
    assert(client.metaValueFrame("test/frame1").comment == "frame1");
    assert(client.metaValueFrame("test/frame1").format 
        == RhIO::FrameFormat::RGB);
    assert(client.metaValueFrame("test/frame2").comment == "frame2");
    assert(client.metaValueFrame("test/frame2").format 
        == RhIO::FrameFormat::BGR);
    assert(client.metaValueFrame("test/frame1").countWatchers == 0);
    client.enableStreamingFrame("test/frame1");
    assert(client.metaValueFrame("test/frame1").countWatchers == 1);
    client.disableStreamingFrame("test/frame1");
    assert(client.metaValueFrame("test/frame1").countWatchers == 0);

    assert(client.metaValueBool(
        "test/paramBool").streamWatchers == 0);
    assert(client.metaValueInt(
        "test/test3/paramInt").streamWatchers == 0);
    client.enableStreamingValue("test/test3/paramInt");
    assert(client.metaValueBool(
        "test/paramBool").streamWatchers == 0);
    assert(client.metaValueInt(
        "test/test3/paramInt").streamWatchers == 1);
    client.disableStreamingValue("test/test3/paramInt");
    assert(client.metaValueBool(
        "test/paramBool").streamWatchers == 0);
    assert(client.metaValueInt(
        "test/test3/paramInt").streamWatchers == 0);
    client.disableStreamingValue("test/test3/paramInt");
    assert(client.metaValueBool(
        "test/paramBool").streamWatchers == 0);
    assert(client.metaValueInt(
        "test/test3/paramInt").streamWatchers == 0);

    assert(client.listAllCommands().size() == 2);
    assert(client.listAllCommands()[0] == "test/command1");
    assert(client.listAllCommands()[1] == "test/test3/command2");
    
    client.enableStreamingStream("test/stream1");
    client.disableStreamingStream("test/stream1");

    //client.save("/", "/tmp/root");
    //client.load("/", "/tmp/root");

    return 0;
}

