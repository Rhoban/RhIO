cmake_minimum_required(VERSION 3.0)
project (RhioClient)

# RhIO source directory
set (RHIO_DIR "${CMAKE_SOURCE_DIR}/../")

# Enable colors ?
option (COLORS "Enable colors" ON)

# Enable curses
option (CURSES "Enable curses" ON)

set (SOURCES
    Command.cpp
    HelpCommand.cpp
    LsCommand.cpp
    CdCommand.cpp
    Shell.cpp
    Terminal.cpp
    Values.cpp

    main.cpp
    )

# Adding rhio subdirectory
add_subdirectory (${RHIO_DIR} rhiolib)
include_directories ("${RHIO_DIR}/Client")
include_directories ("${RHIO_DIR}/Common")
include_directories ("${RHIO_DIR}/Server")

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
        TuneCommand.cpp
        )
    set (EXTRA_LIBS
        ${EXTRA_LIBS}
        ncurses
        )
endif ()

# Adding shell binary
add_executable (rhio ${SOURCES})
target_link_libraries (rhio ${EXTRA_LIBS})
