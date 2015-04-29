#include <iostream>
#include <cassert>
#include "RhIO.hpp"

class Test
{
    public:

        Test() :
            _bind("test"),
            _bind2("test5")
        {
            _bind.bindNew("valueBool", _valueBool)
                ->comment("bool value")
                ->persisted(true);
            _bind.bind("valueInt1", _valueInt);
            _bind.bind("test2/valueFloat1", _valueFloat);
            _bind.bind("valueInt2", _valueInt2);
            _bind.bindNew("test2/test4/valueFloat2", _valueFloat2)
                ->minimum(-100.0);
            _bind.bind("test2/test3/valueStr", _valueStr);
            assert(RhIO::Root.getValueType("test/valueBool") == RhIO::TypeBool);
            assert(RhIO::Root.getValueBool("test/valueBool").comment == "bool value");
            _bind.newStream("output", "a test stream");
            
            _bind2.bindFunc("command1", "test command1", &Test::command1, *this, {"", "42"});
            assert(RhIO::Root.call("test5/command1", {"2", "3"}) == "5");
            assert(RhIO::Root.call("test5/command1", {"2"}) == "44");
            assert(RhIO::Root.call("test5/command1", {})
                .find("USAGE: command1 <int> <int|42> --> <int>") != std::string::npos);
            _bind.bindFunc("command2", "test command2", &Test::command2, *this);
            assert(RhIO::Root.call("test/command2", {}) == "1");
            _bind.bindFunc("command3", "test command3", &Test::command3, *this);
            assert(RhIO::Root.call("test/command3", {"3.0", "2", "toto"}) > "8.99");
            assert(RhIO::Root.call("test/command3", {"3.0", "2", "toto"}) < "9.01");
            
            _bind.bindFunc("command4", "test command4", &Test::command4, *this);
            assert(RhIO::Root.call("test/command4", {}) == "pouet");
        }

        inline void tick1()
        {
            RhIO::Root.setBool("test/valueBool", true);
            _bind.pull();
            assert(_valueBool == true);
            assert(_valueInt == 2);
            assert(_valueFloat == 42.0);
            assert(_valueStr == "str1");
            _valueInt = 3;
            _bind.push();
            _valueFloat = 1.0;
            _bind.out("output") << "test" << std::endl;
        }
        
        inline void tick2()
        {
            assert(_valueFloat == 1.0);
            _bind.pull();
            assert(_valueBool == true);
            assert(_valueInt == 3);
            assert(_valueFloat == 42.0);
            assert(_valueStr == "str2");
        }

        inline int command1(int a, int b)
        {
            return a + b;
        }
        inline bool command2(void)
        {
            return true;
        }
        inline float command3(float a, long b, std::string c)
        {
            return a + b + c.length();
        }
        inline std::string command4()
        {
            return "pouet";
        }

    private:

        bool _valueBool;
        long _valueInt;
        int _valueInt2;
        double _valueFloat;
        float _valueFloat2;
        std::string _valueStr;
        RhIO::Bind _bind;
        RhIO::Bind _bind2;
};

int main()
{
    RhIO::Root.newChild("test");
    RhIO::Root.newInt("test/valueInt1");
    RhIO::Root.newInt("test/valueInt2");
    RhIO::Root.newFloat("test/test2/valueFloat1");
    RhIO::Root.newStr("test/test2/test3/valueStr");

    RhIO::Root.setInt("test/valueInt1", 2);
    RhIO::Root.setFloat("test/test2/valueFloat1", 42.0);
    RhIO::Root.setStr("test/test2/test3/valueStr", "str1");

    Test test;
    test.tick1();
    assert(RhIO::Root.getInt("test/valueInt1") == 3);
    assert(RhIO::Root.getFloat("test/test2/valueFloat1") == 42.0);
    RhIO::Root.setStr("test/test2/test3/valueStr", "str2");
    test.tick2();

    return 0;
}

