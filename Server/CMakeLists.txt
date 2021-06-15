cmake_minimum_required(VERSION 3.16.3)
project(RhIOServer)

#The server automatically start on program startup (RhIO::start())
option(RHIO_SERVER_AUTOSTART "Autostart RhIO server" OFF)
option (BUILD_KID_SIZE_VISION "Build the Vision" ON)

add_library(RhIO SHARED)
target_link_libraries(RhIO PUBLIC
  RhIOCommon
  jsoncpp_lib
  libzmq
  cppzmq
  Threads::Threads
  ${OpenCV_LIBS}
)
target_include_directories(RhIO PUBLIC
  ${OpenCV_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)


if (BUILD_KID_SIZE_VISION)
   target_compile_definitions(RhIO PRIVATE -DVISION_COMPONENT)
endif ()

target_sources(RhIO PRIVATE
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

if(RHIO_SERVER_AUTOSTART)
  target_sources(RhIO PRIVATE src/autostart.cpp)
endif()

#Build Examples
if(RHIO_BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif()
