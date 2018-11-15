###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(rhio_server)

find_package(catkin REQUIRED COMPONENTS
    rhio_common
    uoe_zeromq
)

# What is published by the 'rhio_server package'
catkin_package(
    INCLUDE_DIRS include
    LIBRARIES rhio_server
    CATKIN_DEPENDS rhio_common uoe_zeromq
)

#Option for compiling Examples
option(BUILD_RHIO_EXAMPLES "Build RhIO Examples" OFF)

#The server automatically start on program startup (RhIO::start())
option(RHIO_SERVER_AUTOSTART "Autostart RhIO server" OFF)

#Include sources directory
include_directories(include ${catkin_INCLUDE_DIRS})

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
    yaml-cpp
    pthread
    ${catkin_LIBRARIES}
)

if(RHIO_SERVER_AUTOSTART)
    set (SOURCES ${SOURCES} src/autostart.cpp)
endif()

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Server as shared library
add_library(rhio_server SHARED ${SOURCES})
target_link_libraries(rhio_server ${RHIO_LIBRARIES})

#Build Examples
if (BUILD_RHIO_EXAMPLES)
    add_executable(exampleIntro Examples/exampleIntro.cpp)
    target_link_libraries(exampleIntro rhio_server)

    add_executable(exampleValues Examples/exampleValues.cpp)
    target_link_libraries(exampleValues rhio_server)

    add_executable(exampleStreamsAndCommands Examples/exampleStreamsAndCommands.cpp)
    target_link_libraries(exampleStreamsAndCommands rhio_server)

    add_executable(exampleFrames Examples/exampleFrames.cpp)
    target_link_libraries(exampleFrames rhio_server)

    add_executable(exampleBind Examples/exampleBind.cpp)
    target_link_libraries(exampleBind rhio_server)
endif (BUILD_RHIO_EXAMPLES)

#Install rules
install(TARGETS rhio_server DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

