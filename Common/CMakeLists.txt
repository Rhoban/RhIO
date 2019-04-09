###
### Rhoban Input Output Library: Common part
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(RhIOCommon)

find_package(catkin REQUIRED)

# What is published by the 'RhIOCommon package'
catkin_package(
  INCLUDE_DIRS src
  LIBRARIES RhIOCommon
)

#Include sources directory
include_directories(src/)

#Sources files
set(SOURCES
    src/DataBuffer.cpp
    src/Protocol.cpp
)

#Enable C++17
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Server and Client as shared library
add_library(RhIOCommon SHARED ${SOURCES})
