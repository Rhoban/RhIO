###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(rhio_client)

find_package(catkin REQUIRED COMPONENTS
  uoe_zeromq
  rhio_common
)

# What is published by the 'rhio_client package'
catkin_package(
  INCLUDE_DIRS include
  LIBRARIES rhio_client
  CATKIN_DEPENDS 
    uoe_zeromq
    rhio_common 
)

#Include sources directory
include_directories(include ${catkin_INCLUDE_DIRS})

#Sources files
set(SOURCES
    src/ClientReq.cpp
    src/ClientSub.cpp
)

#Needed Libraries
set(RHIO_LIBRARIES
    pthread
    ${catkin_LIBRARIES}
)

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#Build Client as shared library
add_library(rhio_client SHARED ${SOURCES})
target_link_libraries(rhio_client ${RHIO_LIBRARIES})

#Install rules
install(TARGETS rhio_client DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

