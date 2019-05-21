###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(RhIOClient)

find_package(OpenCV 3.2.0 REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  RhIOCommon
  ZeroMQ
  )

# What is published by the 'RhIOClient package'
catkin_package(
  INCLUDE_DIRS src ${OpenCV_INCLUDE_DIRS}
  LIBRARIES RhIOClient ${OpenCV_LIBS}
  CATKIN_DEPENDS RhIOCommon ZeroMQ
)

#Include sources directory
include_directories(src ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

#Sources files
set(SOURCES
    src/ClientReq.cpp
    src/ClientSub.cpp
)

#Needed Libraries
set(RHIO_LIBRARIES
    pthread
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
)

#Enable C++17
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Server and Client as shared library
add_library(RhIOClient SHARED ${SOURCES})
target_link_libraries(RhIOClient ${RHIO_LIBRARIES})
