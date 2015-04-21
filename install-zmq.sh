#!/bin/bash

dir=$PWD
rm -rf zeromq-4.0.5*
rm -rf ZeroMQ
rm -rf cppzmq

wget http://download.zeromq.org/zeromq-4.0.5.tar.gz &&
tar zxvf zeromq-4.0.5.tar.gz &&
cd zeromq-4.0.5 &&
./configure --prefix=$dir/ZeroMQ &&
make &&
make install &&
cd ..
rm -rf zeromq-4.0.5.tar.gz

echo "Installing CPPZMQ..." &&
git clone https://github.com/zeromq/cppzmq.git &&
ln -s cppzmq/zmq.hpp ZeroMQ/include/zmq.hpp
