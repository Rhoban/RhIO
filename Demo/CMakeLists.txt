###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(RhIOClient)

# Main RhIO directory
set (RHIO_DIRECTORY "${CMAKE_SOURCE_DIR}/../"
    CACHE STRING "Main RhIO directory")

# Adding common dependency
add_subdirectory("${RHIO_DIRECTORY}/Server" server)
include_directories("${RHIO_DIRECTORY}/Server/src")
include_directories("${RHIO_DIRECTORY}/Common/src")

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Server and Client as shared library
add_executable(demo main.cpp)
target_link_libraries(demo RhIO RhIOCommon)
