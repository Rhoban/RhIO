# Getting started

## Getting RhIO

You can get RhIO using git

    git clone --depth=1 https://github.com/Rhoban/RhIO.git

Or download the master version:

* [RhIO master (zip)](https://github.com/Rhoban/RhIO/archive/master.zip)
* [RhIO master (tar.gz)](https://github.com/Rhoban/RhIO/archive/master.tar.gz)

## Building

To build RhIO, you'll need cmake and a C++11 compliant compiler (gcc 4.8 or
above for instance).

    apt-get install cmake g++

Then, you'll first have to get and build ZeroMQ. To avoid compatibility reasons,
RhIO uses its own build of ZeroMQ, that will be done automatically with the 
following script:

    ./install-zmq.sh

You can now build the shell:

    cd Shell
    mkdir build
    cd build
    cmake ..
    make

And, for instance, the `Skeleton` demo:

    cd Skeleton
    mkdir build
    cd build
    cmake ..
    make

You can now run the skeleton project (in Skeleton/build):

    ./skeleton

And the rhio shell (in Shell/build):

    ./rhio


