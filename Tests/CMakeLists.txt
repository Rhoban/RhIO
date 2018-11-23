###
### Rhoban Input Output Libray
### Rhoban 2015
###

cmake_minimum_required(VERSION 2.8)
project(rhio_tests)

find_package(catkin REQUIRED COMPONENTS
    uoe_zeromq
    rhio_server
    rhio_client
)

# What is published by the 'rhio_test package'
catkin_package(
    INCLUDE_DIRS src
    CATKIN_DEPENDS 
        uoe_zeromq
        rhio_server 
        rhio_client 
)

#Include sources directory
include_directories(src ${catkin_INCLUDE_DIRS})

#Needed Libraries
set(RHIO_LIBRARIES
    pthread
    ${catkin_LIBRARIES}
)

#Enable C++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#Enable compiler Warning
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall")
#Enable testing (assert)
set(CMAKE_BUILD_TYPE Debug)

add_executable(testTree src/testTree.cpp)
target_link_libraries(testTree ${RHIO_LIBRARIES})
    
add_executable(testValue src/testValue.cpp)
target_link_libraries(testValue ${RHIO_LIBRARIES})
    
add_executable(testThreadSafeTree src/testThreadSafeTree.cpp)
target_link_libraries(testThreadSafeTree ${RHIO_LIBRARIES})
add_executable(testThreadSafeValue src/testThreadSafeValue.cpp)
target_link_libraries(testThreadSafeValue ${RHIO_LIBRARIES})
add_executable(testThreadSafeValueUpdate src/testThreadSafeValueUpdate.cpp)
target_link_libraries(testThreadSafeValueUpdate ${RHIO_LIBRARIES})
    
add_executable(testPersistSave src/testPersistSave.cpp)
target_link_libraries(testPersistSave ${RHIO_LIBRARIES})
add_executable(testPersistLoad src/testPersistLoad.cpp)
target_link_libraries(testPersistLoad ${RHIO_LIBRARIES})

add_executable(testServerRep src/testServerRep.cpp)
target_link_libraries(testServerRep ${RHIO_LIBRARIES})
add_executable(testClientReq src/testClientReq.cpp)
target_link_libraries(testClientReq ${RHIO_LIBRARIES})

add_executable(testServerPub src/testServerPub.cpp)
target_link_libraries(testServerPub ${RHIO_LIBRARIES})
add_executable(testClientSub src/testClientSub.cpp)
target_link_libraries(testClientSub ${RHIO_LIBRARIES})

add_executable(testServer src/testServer.cpp)
target_link_libraries(testServer ${RHIO_LIBRARIES})

add_executable(testServerHighFreq src/testServerHighFreq.cpp)
target_link_libraries(testServerHighFreq ${RHIO_LIBRARIES})
add_executable(testClientHighFreq src/testClientHighFreq.cpp)
target_link_libraries(testClientHighFreq ${RHIO_LIBRARIES})

add_executable(testServerStress src/testServerStress.cpp)
target_link_libraries(testServerStress ${RHIO_LIBRARIES})

add_executable(testCommands src/testCommands.cpp)
target_link_libraries(testCommands ${RHIO_LIBRARIES})

add_executable(testBind src/testBind.cpp)
target_link_libraries(testBind ${RHIO_LIBRARIES})

add_executable(testImgs src/testImgs.cpp)
target_link_libraries(testImgs ${RHIO_LIBRARIES})

add_executable(benchValues src/benchValues.cpp)
target_link_libraries(benchValues ${RHIO_LIBRARIES})

