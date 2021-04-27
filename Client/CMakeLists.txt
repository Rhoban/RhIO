###
### Rhoban Input Output Libray
### Rhoban 2015
###
cmake_minimum_required(VERSION 3.16.3)
project(RhIOClient)

#Build Server and Client as shared library
add_library(RhIOClient SHARED
    src/ClientReq.cpp
    src/ClientSub.cpp
)
target_include_directories(RhIOClient PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(RhIOClient PRIVATE
    ${OpenCV_LIBS}
    libzmq
    Threads::Threads
)
