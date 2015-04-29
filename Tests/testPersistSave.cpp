#include <iostream>
#include <cassert>
#include "RhIO.hpp"
#include "Filesystem.hpp"

int main() 
{
    RhIO::Root.newChild("test");
    RhIO::Root.newChild("test2/test3");

    RhIO::Root.newBool("paramBool")
        ->comment("bool parameter")
        ->persisted(true)
        ->defaultValue(true);
    RhIO::Root.newInt("test/paramInt")
        ->persisted(true)
        ->minimum(-1)
        ->maximum(1024)
        ->defaultValue(42);
    RhIO::Root.newFloat("test/paramFloat")
        ->comment("float parameter")
        ->minimum(0.0)
        ->persisted(true);
    RhIO::Root.newStr("test2/test3/paramStr")
        ->comment("str parameter")
        ->persisted(true)
        ->defaultValue("off");
    
    RhIO::Root.setFloat("test/paramFloat", 3.14);
    assert(RhIO::Root.getValueFloat("test/paramFloat").valuePersisted == 0.0);
    RhIO::Root.save("/tmp/testRhIO");
    assert(RhIO::Root.getValueFloat("test/paramFloat").valuePersisted == 3.14);

    return 0;
}

