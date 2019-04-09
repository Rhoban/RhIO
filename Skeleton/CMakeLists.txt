cmake_minimum_required (VERSION 2.8)
project (skeleton)
find_package(Deps)

deps_add_library("rhoban/rhio")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable (skeleton main.cpp)
target_link_libraries (skeleton ${DEPS_LIBRARIES})
