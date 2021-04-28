cmake_minimum_required(VERSION 3.16.3)
project(RhIO
  DESCRIPTION "Rhoban Input Output Library, Rhoban 2015"
  LANGUAGES CXX
)

option(RHIO_BUILD_TESTS "Build RhIO Tests" OFF)
option(RHIO_BUILD_EXAMPLES "Build RhIO Examples" OFF)

include(cmake/CPM.cmake)

find_package(OpenCV 4.2.0 REQUIRED)
find_package(Threads REQUIRED)

CPMAddPackage(
  NAME libzmq
  GITHUB_REPOSITORY zeromq/libzmq
  VERSION 4.3.4
  OPTIONS
    "BUILD_TESTS OFF"
    "WITH_PERF_TOOL OFF"
    "LIBZMQ_PEDANTIC OFF"
)

CPMAddPackage(
  NAME cppzmq
  GITHUB_REPOSITORY zeromq/cppzmq
  VERSION 4.7.1
  OPTIONS
    "CPPZMQ_BUILD_TESTS OFF"
)

CPMAddPackage(
  NAME jsoncpp
  GITHUB_REPOSITORY open-source-parsers/jsoncpp
  GIT_TAG 1.9.4
  OPTIONS
    "JSONCPP_WITH_TESTS OFF"
    "BUILD_STATIC_LIBS OFF"
)

#Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
#set(BUILD_SHARED_LIBS ON)
#Enable compiler Warning
add_compile_options(-W -Wall -Wno-reorder)

add_subdirectory(Common)
add_subdirectory(Server)
add_subdirectory(Client)
add_subdirectory(Shell)
#add_subdirectory(Skeleton)

if(BUILD_TESTING AND RHIO_BUILD_TESTS)
  add_subdirectory(Tests)
endif()
