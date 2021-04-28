cmake_minimum_required(VERSION 3.16.3)

add_executable(testTree testTree.cpp)
target_link_libraries(testTree RhIO)

add_executable(testValue testValue.cpp)
target_link_libraries(testValue RhIO)

add_executable(testThreadSafeTree testThreadSafeTree.cpp)
target_link_libraries(testThreadSafeTree RhIO)
add_executable(testThreadSafeValue testThreadSafeValue.cpp)
target_link_libraries(testThreadSafeValue RhIO)
add_executable(testThreadSafeValueUpdate testThreadSafeValueUpdate.cpp)
target_link_libraries(testThreadSafeValueUpdate RhIO)

add_executable(testPersistSave testPersistSave.cpp)
target_link_libraries(testPersistSave RhIO)
add_executable(testPersistLoad testPersistLoad.cpp)
target_link_libraries(testPersistLoad RhIO)

add_executable(testServerRep testServerRep.cpp)
target_link_libraries(testServerRep RhIO)
add_executable(testClientReq testClientReq.cpp)
target_link_libraries(testClientReq RhIOClient)

add_executable(testServerPub testServerPub.cpp)
target_link_libraries(testServerPub RhIO)
add_executable(testClientSub testClientSub.cpp)
target_link_libraries(testClientSub RhIOClient)

add_executable(testServer testServer.cpp)
target_link_libraries(testServer RhIO)

add_executable(testServerStress testServerStress.cpp)
target_link_libraries(testServerStress RhIO)

add_executable(testCommands testCommands.cpp)
target_link_libraries(testCommands RhIO)

add_executable(testBind testBind.cpp)
target_link_libraries(testBind RhIO)

add_executable(testImgs testImgs.cpp)
target_link_libraries(testImgs RhIO)
