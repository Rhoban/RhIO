cmake_minimum_required(VERSION 2.8)
project (RhioClient)

# RhIO source directory
set (RHIO_DIR "${CMAKE_SOURCE_DIR}/../")

# Enable colors ?
option (COLORS "Enable colors" ON)

# Enable curses
option (CURSES "Enable curses" ON)

#Include ZMQ (Network Library)
set(ZMQ_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../ZeroMQ" CACHE STRING "ZMQ lib directory")
include_directories(${ZMQ_DIRECTORY}/include)
link_directories(${ZMQ_DIRECTORY}/lib)

set (SOURCES
    commands/Command.cpp
    commands/ClearCommand.cpp
    commands/HelpCommand.cpp
    commands/LsCommand.cpp
    commands/WatchCommand.cpp
    commands/CdCommand.cpp
    commands/LogCommand.cpp
    commands/SyncCommand.cpp
    commands/RemoteCommand.cpp
    commands/PlotCommand.cpp
    commands/DiffCommand.cpp
    commands/SaveCommand.cpp
    commands/LoadCommand.cpp
    commands/TreeCommand.cpp
    commands/CatCommand.cpp
    commands/RepeatCommand.cpp
    GnuPlot.cpp
    Shell.cpp
    CSV.cpp
    Terminal.cpp
    NodePool.cpp
    StreamManager.cpp
    Completion.cpp
    Node.cpp

    main.cpp
    )

# Adding rhio subdirectory
add_subdirectory (${RHIO_DIR} rhiolib)
include_directories ("${CMAKE_SOURCE_DIR}")
include_directories ("${RHIO_DIR}/Client")
include_directories ("${RHIO_DIR}/Common")
include_directories ("${RHIO_DIR}/Server")
include_directories ("${RHIO_DIR}/include")
include_directories ("${RHIO_DIR}/contrib")

set (EXTRA_LIBS
    RhIOClient
    )

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -std=c++11")

if (COLORS)
    add_definitions(-DHAS_COLORS)
endif ()

# Adding curses sources and library
if (CURSES)
    add_definitions(-DHAS_CURSES)
    set (SOURCES
        ${SOURCES}
        commands/TuneCommand.cpp
        Curse.cpp
        )
    set (EXTRA_LIBS
        ${EXTRA_LIBS}
        ncurses form panel
        )
endif ()

# Adding shell binary
add_executable (rhio ${SOURCES})
target_link_libraries (rhio ${EXTRA_LIBS})

install(TARGETS rhio DESTINATION bin)
