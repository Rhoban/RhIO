# For build tree configuration.
get_target_property( JsonCpp_LIBRARY json LOCATION )
set( JsonCpp_INCLUDE ${JsonCpp_SOURCE_DIR}/include )
configure_file( "${JsonCpp_SOURCE_DIR}/cmake/JsonCppConfig.cmake.in"
  "${JsonCpp_BINARY_DIR}/JsonCppConfig.cmake"
  )

# For install configuration.
get_filename_component( JsonCpp_INSTALL_LIBRARY
  ${JsonCpp_LIBRARY} NAME
  )
set( JsonCpp_LIBRARY ${CMAKE_INSTALL_PREFIX}/lib/${JsonCpp_INSTALL_LIBRARY} )
set( JsonCpp_INCLUDE ${CMAKE_INSTALL_PREFIX}/include )
configure_file( "${JsonCpp_SOURCE_DIR}/cmake/JsonCppConfig.cmake.in"
  "${JsonCpp_BINARY_DIR}/InstallFiles/JsonCppConfig.cmake"
  )
install(FILES
  "${JsonCpp_BINARY_DIR}/InstallFiles/JsonCppConfig.cmake"
  DESTINATION lib
  COMPONENT Development
  )
