#!/bin/bash

version=4.0.5
dir=$PWD
rm -rf zeromq-${version}*
rm -rf ZeroMQ
rm -rf cppzmq

# Installing ZMQ
wget http://download.zeromq.org/zeromq-${version}.tar.gz &&
tar zxvf zeromq-${version}.tar.gz &&
cd zeromq-${version} &&
./configure --prefix=$dir/ZeroMQ &&
make &&
make install &&
cd ..
rm -rf zeromq-${version}
rm -rf zeromq-${version}.tar.gz

# Installing CPP ZMQ binding
git clone https://github.com/zeromq/cppzmq.git &&
ln -s cppzmq/zmq.hpp ZeroMQ/include/zmq.hpp
