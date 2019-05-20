cmake_minimum_required(VERSION 2.8)
project (RhIOShell)

find_package(OpenCV 3.2.0 REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  RhIOClient
  jsoncpp
  )

catkin_package(
  INCLUDE_DIRS .
  CATKIN_DEPENDS RhIOClient jsoncpp
)

# Enable colors ?
option (COLORS "Enable colors" ON)

# Enable curses
option (CURSES "Enable curses (libncurses5-dev)" ON)

# Enable log img
option (LOGIMG "Enable log img (libgd-dev required)" OFF)

set (SOURCES
    src/commands/Command.cpp
    src/commands/ClearCommand.cpp
    src/commands/HelpCommand.cpp
    src/commands/LsCommand.cpp
    src/commands/WatchCommand.cpp
    src/commands/CdCommand.cpp
    src/commands/LogCommand.cpp
    src/commands/SyncCommand.cpp
    src/commands/RemoteCommand.cpp
    src/commands/PlotCommand.cpp
    src/commands/Plot2DCommand.cpp
    src/commands/Plot3DCommand.cpp
    src/commands/DiffCommand.cpp
    src/commands/SaveCommand.cpp
    src/commands/LoadCommand.cpp
    src/commands/TreeCommand.cpp
    src/commands/CatCommand.cpp
    src/commands/ViewCommand.cpp
    src/commands/RepeatCommand.cpp
    src/commands/DelayCommand.cpp
    src/commands/PadCommand.cpp
    src/joystick/Joystick.cpp
    src/GnuPlot.cpp
    src/FrameStreamViewer.cpp
    src/Shell.cpp
    src/CSV.cpp
    src/Terminal.cpp
    src/NodePool.cpp
    src/StreamManager.cpp
    src/Completion.cpp
    src/Node.cpp
    src/main.cpp
)

include_directories (src ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

set (EXTRA_LIBS
    pthread 
    ${catkin_LIBRARIES}
    ${OpenCV_LIBS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wno-reorder")

if (COLORS)
    add_definitions(-DHAS_COLORS)
endif ()

# Logging img
if (LOGIMG)
    add_definitions(-DHAS_LOGIMG)
    set (SOURCES
        ${SOURCES}
        src/commands/LogImgCommand.cpp
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
        src/commands/TuneCommand.cpp
        src/Curse.cpp
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

