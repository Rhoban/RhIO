cmake_minimum_required(VERSION 3.16.3)
project(RhIOClient)

#Build Server and Client as shared library
add_library(RhIOClient SHARED
    src/ClientReq.cpp
    src/ClientSub.cpp
)

target_include_directories(RhIOClient PUBLIC
  ${OpenCV_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)
target_link_libraries(RhIOClient PUBLIC
    ${OpenCV_LIBS}
    cppzmq
    Threads::Threads
    RhIOCommon
)
