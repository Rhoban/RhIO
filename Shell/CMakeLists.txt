cmake_minimum_required(VERSION 2.8)
project (RhioClient)

# RhIO source directory
set (RHIO_DIR "${CMAKE_SOURCE_DIR}/../")

# Enable colors ?
option (COLORS "Enable colors" ON)

# Enable curses
option (CURSES "Enable curses (libncurses5-dev)" ON)

# Enable log img
option (LOGIMG "Enable log img (libgd-dev required)" OFF)

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
    commands/Plot2DCommand.cpp
    commands/Plot3DCommand.cpp
    commands/DiffCommand.cpp
    commands/SaveCommand.cpp
    commands/LoadCommand.cpp
    commands/TreeCommand.cpp
    commands/CatCommand.cpp
    commands/ViewCommand.cpp
    commands/RepeatCommand.cpp
    commands/DelayCommand.cpp
    commands/PadCommand.cpp
    joystick/Joystick.cpp
    GnuPlot.cpp
    FrameStreamViewer.cpp
    Shell.cpp
    CSV.cpp
    Terminal.cpp
    NodePool.cpp
    StreamManager.cpp
    Completion.cpp
    Node.cpp
    main.cpp
)

include_directories ("${CMAKE_SOURCE_DIR}")
include_directories ("${RHIO_DIR}")
link_directories ("${RHIO_DIR}/build")

set (EXTRA_LIBS
    RhIOClient
    jsoncpp 
    pthread 
    zmq
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wno-reorder")

if (COLORS)
    add_definitions(-DHAS_COLORS)
endif ()

# Logging img
if (LOGIMG)
    add_definitions(-DHAS_LOGIMG)
    set (SOURCES
        ${SOURCES}
        commands/LogImgCommand.cpp
        )
    set (EXTRA_LIBS
        ${EXTRA_LIBS}
        gd
    )
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

