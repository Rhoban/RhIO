###
### Rhoban Input Output Library: Common part
### Rhoban 2015
###

cmake_minimum_required(VERSION 3.16.3)
project(RhIOCommon)

#Build Server and Client as shared library
add_library(RhIOCommon SHARED
  src/DataBuffer.cpp
  src/Protocol.cpp
)

target_compile_features(RhIOCommon PRIVATE cxx_std_17)
target_include_directories(RhIOCommon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
