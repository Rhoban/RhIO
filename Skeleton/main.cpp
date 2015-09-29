#include <math.h>
#include <iostream>
#include <unistd.h>
#include <RhIO.hpp>
#include <sstream>

using namespace std;

int main()
{
    float amplitude, s, t;

    RhIO::Bind bind("test");
    RhIO::Root.load("rhio");

    bind.bindNew("amplitude", amplitude, RhIO::Bind::PullOnly)
        ->defaultValue(1.0)->persisted(true)
        ->minimum(0)->maximum(15);

    bind.bindNew("t", t, RhIO::Bind::PushOnly);
    bind.bindNew("s", s, RhIO::Bind::PushOnly);

    t = 0.0;
    while (true) {
        bind.pull();
        t += 0.02;
        s = sin(t)*amplitude;
        usleep(20000);
        bind.push();
    }
}
