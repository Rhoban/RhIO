#include <cmath>
#include <iostream>
#include <unistd.h>
#include <RhIO.hpp>

class MySine
{
    public:
        MySine()
            : t(0), value(0), amplitude(1), phase(1), bind("sine")
        {
            bind.bindNew("t", t, RhIO::Bind::PushOnly)
                ;
            bind.bindNew("value", value, RhIO::Bind::PushOnly)
                ->comment("Sine output")
                ;
            bind.bindNew("amplitude", value, RhIO::Bind::PullOnly)
                ->persisted(true)
                ;
            bind.bindNew("phase", phase, RhIO::Bind::PullOnly)
                ->comment("Sine phase")
                ;
            
            bind.bindFunc("reset", "Reset the time",
                &MySine::reset, *this);
        }

        void tick()
        {
            t += 0.01;
            bind.pull();
            value = amplitude*sin(t + phase);
            bind.push();
        }

        void reset()
        {
            t = 0;
        }

        float t, value, amplitude, phase;
        RhIO::Bind bind;
};

int main()
{
    MySine sine;

    while (true) {
        sine.tick();
        usleep(10000);
    }
}
