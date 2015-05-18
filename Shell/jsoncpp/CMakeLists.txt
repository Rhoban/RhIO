cmake_minimum_required( VERSION 2.8 )
project( JsonCpp )

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "")
endif ()

if( NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${JsonCpp_BINARY_DIR}/bin )
endif()
if( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${JsonCpp_BINARY_DIR}/lib )
endif()
if( NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY )
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${JsonCpp_BINARY_DIR}/lib )
endif()

include_directories( include )

add_subdirectory( include )
add_subdirectory( src )

include( CTest )
if( BUILD_TESTING )
#  add_subdirectory( test )
endif()

add_subdirectory( cmake )
