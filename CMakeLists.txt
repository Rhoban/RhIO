cmake_minimum_required(VERSION 3.16.3)
project(RhIO
  DESCRIPTION "Rhoban Input Output Library, Rhoban 2015"
  LANGUAGES CXX
)

option(RHIO_BUILD_TESTS "Build RhIO Tests" OFF)
option(RHIO_BUILD_EXAMPLES "Build RhIO Examples" OFF)

find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)

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
