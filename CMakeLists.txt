cmake_minimum_required(VERSION 3.16.3)
project(Rhio LANGUAGES CXX)

option(RHIO_BUILD_TESTS "Build the unit tests if BUILD_TESTING is enable." ${BUILD_TESTING})
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
)

CPMAddPackage(
  NAME jsoncpp
  GITHUB_REPOSITORY open-source-parsers/jsoncpp
  GIT_TAG 1.9.4
  OPTIONS
    "JSONCPP_WITH_TESTS OFF"
)

#Enable C++17
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")

#add_subdirectory(Backup)
add_subdirectory(Client)
add_subdirectory(Common)
#add_subdirectory(Server)
#add_subdirectory(Shell)
#add_subdirectory(Skeleton)

include(CTest)
if(BUILD_TESTING AND RHIO_BUILD_TESTS)
  add_subdirectory(tests)
endif()
