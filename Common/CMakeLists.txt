###
### Rhoban Input Output Library: Common part
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(RhIOCommon)

#Include sources directory
include_directories(src/)

#Sources files
set(SOURCES
    src/DataBuffer.cpp
    src/Protocol.cpp
)

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Server and Client as shared library
add_library(RhIOCommon SHARED ${SOURCES})
