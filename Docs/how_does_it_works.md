# How does it works?

## Server architecture

RhIO uses internally two ZMQ servers: one REP/REQ and one PUB/SUB. Basically, the
REQ/REP is used to answer commands and the PUB/SUB is used to subscribe to parameter
changes (think for instance of the ``watch`` or ``plot`` command in the [shell](shell.md).

You can notify RhIO that you want a parameter to be streamed to its PUB/SUB server
using a command in the REQ/REP one. In the same way, you can tell it that you no
longer want to receive this value. Internally, RhIO counts how many clients are
interrested in getting a parameter streamed, and stream it if at least one asks
for it.

## Protocol

RhIO is based on a custom binary protocol, that can be found under the repository ``Common/``
directory.

## Internal types

All floating numbers are internally represented using double precision floating 
points, and all ints are 64-bits.


