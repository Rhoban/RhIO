# Getting started

## Installing

RhIO now used [deps](https://github.com/rhoban/deps), which is a lightweight package
manager used to handle dependencies.

First, you should [install deps](https://github.com/rhoban/deps#installation), then type:

    deps install rhoban/rhio

RhIO and its dependencies will be downloaded and installed.

## Building the example

There is an example in the `Skeleton` directory, you can build it to try RhIO:

    cd deps/packages/rhoban_rhio/Skeleton/
    mkdir build
    cd build
    cmake ..
    make

And then run the skeleton:

    ./skeleton

And run RhIO in another shell:

    rhio

Have a look to the [source of skeleton](/Skeleton) to understand how it works.
