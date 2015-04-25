#include <iostream>
#include <cassert>
#include "RhIO.hpp"

class Test
{
    public:

        Test()
        {
            _bind.bind("test/valueBool", _valueBool);
            _bind.bind("valueInt1", _valueInt);
            _bind.bind("test/test2/valueFloat1", _valueFloat);
            _bind.bind("valueInt2", _valueInt2);
            _bind.bind("test/test2/valueFloat2", _valueFloat2);
            _bind.bind("test/valueStr", _valueStr);
        }

        inline void tick1()
        {
            _bind.pull();
            assert(_valueBool == true);
            assert(_valueInt == 2);
            assert(_valueFloat == 42.0);
            assert(_valueStr == "str1");
            _valueInt = 3;
            _bind.push();
            _valueFloat = 1.0;
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

    private:

        bool _valueBool;
        long _valueInt;
        int _valueInt2;
        double _valueFloat;
        float _valueFloat2;
        std::string _valueStr;
        RhIO::Bind _bind;
};

int main()
{
    RhIO::Root.newChild("test");
    RhIO::Root.newBool("test/valueBool");
    RhIO::Root.newInt("valueInt1");
    RhIO::Root.newInt("valueInt2");
    RhIO::Root.newFloat("test/test2/valueFloat1");
    RhIO::Root.newFloat("test/test2/valueFloat2");
    RhIO::Root.newStr("test/valueStr");

    RhIO::Root.setBool("test/valueBool", true);
    RhIO::Root.setInt("valueInt1", 2);
    RhIO::Root.setFloat("test/test2/valueFloat1", 42.0);
    RhIO::Root.setStr("test/valueStr", "str1");

    Test test;
    test.tick1();
    assert(RhIO::Root.getInt("valueInt1") == 3);
    assert(RhIO::Root.getFloat("test/test2/valueFloat1") == 42.0);
    RhIO::Root.setStr("test/valueStr", "str2");
    test.tick2();

    return 0;
}

