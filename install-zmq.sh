#!/bin/bash

version=3.2.5
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
