###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(RhIOServer)

# Main RhIO directory
set (RHIO_DIRECTORY "${CMAKE_SOURCE_DIR}/../"
    CACHE STRING "Main RhIO directory")

#The server automatically start on program startup (RhIO::start())
option(RHIO_SERVER_AUTOSTART "Autostart RhIO server" ON)

# Adding common dependency
add_subdirectory("${RHIO_DIRECTORY}/Common" common)
include_directories("${RHIO_DIRECTORY}/Common/src")

#Sources files
set(SOURCES
    src/Bind.cpp
    src/CommandNode.cpp
    src/IONode.cpp
    src/Filesystem.cpp
    src/RhIO.cpp
    src/ServerPub.cpp
    src/ServerRep.cpp
    src/Stream.cpp
    src/StreamNode.cpp
    src/FrameNode.cpp
    src/ValueNode.cpp
)

#Needed Libraries
set(RHIO_LIBRARIES
    pthread
    zmq
)

if(RHIO_SERVER_AUTOSTART)
    set (SOURCES ${SOURCES} src/autostart.cpp)
endif()

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Server and Client as shared library
add_library(RhIO SHARED ${SOURCES})
target_link_libraries(RhIO ${RHIO_LIBRARIES})
