#include <iostream>
#include "rhio_server/RhIO.hpp"

/**
 * Binding is a set of shortcuts 
 * internally using Values, Streams and Commands
 * designed to be used inside a class.
 */

class Object
{
    public:

        /**
         * Constructor
         */
        Object() :
            //The binder is initialized with a path
            //in RhIO tree. This path will be
            //the default "working" node.
            //Non existing path is created.
            _binder("path/in/tree"),
            _attrStr(),
            _attrDouble(),
            _attrInt()
        {
            //Instance attributs can be binded to 
            //specific named values.
            //The value is created and then binding is done.
            //The shortcut node() retrieve the IONode
            //defined by binder initialization path path/in/tree.
            _binder.node().newStr("paramStr")
                ->comment("description paramStr");
            //Many class instances can bind the same
            //already created value
            _binder.bind("paramStr", _attrStr);

            //The shortcut bindNew() both create
            //the given value name and bind given attribute.
            //Type is automatically found.
            //BindNew() returns a ValueBuilder exactly like
            //newBool, newInt, newFloat and newStr.
            _binder.bindNew("paramDouble", _attrDouble)
                ->comment("description paramDouble");

            //Sub path also works
            //Special Push/Pull policy can be specified.
            //Default is RhIO::Bind::PushAndPull. 
            //If RhIO::Bind::PushOnly is set, pull is disable for
            //this variable (pull() method does not update it).
            //If RhIO::Bind::PullOnly is set, push is disable for
            //this variable (push() method does not publish it).
            _binder.bindNew("dir/paramInt", _attrInt, RhIO::Bind::PushOnly)
                ->comment("description paramInt");

            //A newStream() shortcut similar to
            //RhIO::Root.newStream() create a new stream
            //directly on binder sub node.
            _binder.newStream("stream1", "description stream1");

            //A newFrame() shortcut similar to
            //RhIO::Root.newFrame() create a new frame
            //directly on binder sub node.
            _binder.newFrame("frame1", "description frame1", 
                RhIO::FrameFormat::RGB);

            //BindFunc() is the shortcut for RhIO::Root.newCommand()
            //with generic type inference. The class member method
            //member1 is externally explose to be remotely call
            //by the client. First argument is the command name, 
            //second argument is an helpful description message.
            //Third and fourth arguments are class member function
            //pointer and reference to associated instance.
            //Function return type and arguments type are automatically
            //inferred. Command string arguments parsing and conversion
            //to function argument types is automatically generated.
            _binder.bindFunc("command1", "description command1", 
                &Object::member1, *this);

            //Default values to function can also be specified.
            //Last bindFunc() arguments is a vector of string
            //of the same size of function number of arguments.
            //Default values are given in textual format or empty
            //string to not give default.
            _binder.bindFunc("command2", "description command2", 
                &Object::member2, *this, {"3.14", "true"});
        }

        /**
         * Worker function
         */
        void process()
        {
            //Binded values are existing in two copies.
            //One copy into instance attributes, the other
            //copy into RhIO global value. They need to
            //be synchronized.

            //Pull() assigns all binded attributes to 
            //current RhIO global values of reference.
            //Without pull(), instance attributes  can not
            //be updated by external client or RhIO.
            _binder.pull();

            //Retrieve values
            std::cout << "paramDouble: " << _attrDouble << std::endl;
            //Update values
            _attrDouble = 3.14;
            _attrInt = 42;

            //Write some debug information using out()
            //shortcut similar to RhIO::Root.out().
            _binder.out("stream1") << "debug information" << std::endl;

            //Push() assigns all binded RhIO global values
            //to current instance attributes.
            //Without push(), attributes updates are not
            //publish to external client or RhIO.
            _binder.push();
        }

        /**
         * Example members methods which will be
         * called from remote client
         */
        int member1(int a)
        {
            return a + 1;
        }
        std::string member2(double a, bool b)
        {
            if (b) {
                return "argument true: " + std::to_string(a);
            } else {
                return "argument false: " + std::to_string(a);
            }
        }

    private:

        /**
         * RhIO Binder instance
         */
        RhIO::Bind _binder;

        /**
         * Some class attributs
         */
        std::string _attrStr;
        double _attrDouble;
        int _attrInt;
};

int main()
{
    //Start server
    if (!RhIO::started()) {
        RhIO::start();
    }

    //Initialize the instance
    Object obj;

    //Externally update values
    RhIO::Root.setFloat("path/in/tree/paramDouble", 10.0);

    //Call instance worker function
    obj.process();

    //Display updated values
    std::cout << "paramDouble: " << 
        RhIO::Root.getFloat("path/in/tree/paramDouble") << std::endl;

    return 0;
}

