set( test_lib_json_SRCS
  main.cpp
  jsontest.cpp
  )
add_executable( test_lib_json ${test_lib_json_SRCS} )
target_link_libraries( test_lib_json json )
