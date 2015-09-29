cmake_minimum_required (VERSION 2.8)
project (skeleton)

set (RHIO_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/../")

# Adding ZeroMQ
option(RHIO_STATIC_ZMQ
    "Should RhIO link statically with ZMQ?" ON)
set(ZMQ_DIRECTORY "${RHIO_DIRECTORY}/ZeroMQ" CACHE STRING "ZMQ lib directory")
include_directories(${ZMQ_DIRECTORY}/include)
include_directories(${JSONCPP_DIR}/include)
link_directories(${ZMQ_DIRECTORY}/lib)

# Adding RhIO
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_subdirectory ("${RHIO_DIRECTORY}" rhio)
include_directories ("${RHIO_DIRECTORY}/include/")

add_executable (skeleton main.cpp)
target_link_libraries (skeleton RhIO)
