###
### Rhoban Input Output Library: Common part
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(rhio_common)

find_package(catkin REQUIRED)

# What is published by the 'rhio_common package'
catkin_package(
  INCLUDE_DIRS src
  LIBRARIES rhio_common
)

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

#Build Common as shared library
add_library(rhio_common SHARED ${SOURCES})

