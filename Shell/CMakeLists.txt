cmake_minimum_required(VERSION 3.16.3)
project (RhIOShell)

# Enable colors ?
option (COLORS "Enable colors" ON)

# Enable curses
option (CURSES "Enable curses (libncurses5-dev)" ON)

# Enable log img
option (LOGIMG "Enable log img (libgd-dev required)" OFF)

find_package(jsoncpp REQUIRED)

add_executable(rhio)
target_include_directories(rhio PRIVATE ${OpenCV_INCLUDE_DIRS} src/)
target_link_libraries(rhio PRIVATE
  jsoncpp_lib
  Threads::Threads
  RhIOClient
  ${OpenCV_LIBS}
)
target_sources(rhio PRIVATE
    src/commands/Command.cpp
    src/commands/ClearCommand.cpp
    src/commands/HelpCommand.cpp
    src/commands/LsCommand.cpp
    src/commands/ErrCommand.cpp
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

if (COLORS)
    target_compile_definitions(rhio PRIVATE -DHAS_COLORS)
endif ()

# Logging img
if (LOGIMG)
    target_compile_definitions(rhio PRIVATE -DHAS_LOGIMG)
    target_sources(rhio PRIVATE src/commands/LogImgCommand.cpp)
    target_link_libraries(rhio PRIVATE gd)
endif ()

# Adding curses sources and library
if (CURSES)
    target_compile_definitions(rhio PRIVATE -DHAS_CURSES)
    target_sources(rhio PRIVATE
        src/commands/TuneCommand.cpp
        src/Curse.cpp
    )
    target_link_libraries(rhio PRIVATE ncurses form panel)
endif ()

install(TARGETS rhio DESTINATION bin)
