#include <math.h>
#include <iostream>
#include <unistd.h>
#include <RhIO.hpp>
#include <sstream>

using namespace std;

int main()
{
    float amplitude, s, t, freq;

    RhIO::Bind bind("test");
    // Uncomment to enable persisting, you'll have to create a "rhio" directory
    // in the same directory as the binary
    // RhIO::Root.load("rhio");

    // Parameters that can be changed by the user
    bind.bindNew("amplitude", amplitude, RhIO::Bind::PullOnly)
        ->defaultValue(1.0)->persisted(true)
        ->minimum(0)->maximum(15);
    bind.bindNew("freq", freq, RhIO::Bind::PullOnly)
        ->defaultValue(1.0)->persisted(true)
        ->minimum(0);

    // Adding a "status" command that will be available in the shell
    bind.node().newCommand("status", "Demo status command", 
        [&amplitude, &freq](std::vector<std::string>) -> std::string {
        std::stringstream ss;
        ss << "The amplitude is " << amplitude << " and the freq is " << freq;
        return ss.str();
    });

    // Parameters that are monitored only
    bind.bindNew("t", t, RhIO::Bind::PushOnly);
    bind.bindNew("s", s, RhIO::Bind::PushOnly);

    t = 0.0;
    while (true) {
        bind.pull();
        t += 0.02*freq;
        s = sin(t)*amplitude;
        usleep(20000);
        bind.push();
    }
}
