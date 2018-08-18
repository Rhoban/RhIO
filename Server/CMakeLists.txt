###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(RhIOServer)

find_package(catkin REQUIRED COMPONENTS
  RhIOCommon
  ZeroMQ
  )

# What is published by the 'RhIOServer package'
catkin_package(
  INCLUDE_DIRS src
  LIBRARIES RhIO
  CATKIN_DEPENDS RhIOCommon ZeroMQ
)

#Option for compiling Examples
option(BUILD_RHIO_EXAMPLES "Build RhIO Examples" OFF)

#The server automatically start on program startup (RhIO::start())
option(RHIO_SERVER_AUTOSTART "Autostart RhIO server" ON)

#
option (BUILD_KID_SIZE_VISION "Build the Vision" ON)
if (BUILD_KID_SIZE_VISION)
   add_definitions(-DVISION_COMPONENT)
endif ()


#Include sources directory
include_directories(src ${catkin_INCLUDE_DIRS})

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
    ${catkin_LIBRARIES}
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

#Build Examples
if (BUILD_RHIO_EXAMPLES)
    add_executable(exampleIntro Examples/exampleIntro.cpp)
    target_link_libraries(exampleIntro RhIO)

    add_executable(exampleValues Examples/exampleValues.cpp)
    target_link_libraries(exampleValues RhIO)

    add_executable(exampleStreamsAndCommands Examples/exampleStreamsAndCommands.cpp)
    target_link_libraries(exampleStreamsAndCommands RhIO)

    add_executable(exampleFrames Examples/exampleFrames.cpp)
    target_link_libraries(exampleFrames RhIO)

    add_executable(exampleBind Examples/exampleBind.cpp)
    target_link_libraries(exampleBind RhIO)
endif (BUILD_RHIO_EXAMPLES)
