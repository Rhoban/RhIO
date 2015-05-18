#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "Joystick.h"

namespace RhIO
{
    bool Joystick::JoystickEvent::isPressed()
    {
        return value;
    }

    float Joystick::JoystickEvent::getValue()
    {
        return value/(float(1<<15));
    }

    Joystick::Joystick()
      : fd(-1)
    {
    }

    bool Joystick::open()
    {
        fd = ::open(JOYSTICK_DEVNAME, O_RDONLY | O_NONBLOCK); /* read write for force feedback? */

        return fd > 0;
    }

    bool Joystick::getEvent(JoystickEvent *evt)
    {
        int bytes = read(fd, evt, sizeof(evt));

        return (bytes == sizeof(*evt));
    }

    void Joystick::close()
    {
        if (fd > 0) {
            ::close(fd);
            fd = -1;
        }
    }
}
