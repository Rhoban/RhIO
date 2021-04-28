cmake_minimum_required(VERSION 3.16.3)

add_executable(exampleIntro exampleIntro.cpp)
target_link_libraries(exampleIntro RhIO)

add_executable(exampleValues exampleValues.cpp)
target_link_libraries(exampleValues RhIO)

add_executable(exampleStreamsAndCommands exampleStreamsAndCommands.cpp)
target_link_libraries(exampleStreamsAndCommands RhIO)

add_executable(exampleFrames exampleFrames.cpp)
target_link_libraries(exampleFrames RhIO)

add_executable(exampleBind exampleBind.cpp)
target_link_libraries(exampleBind RhIO)
