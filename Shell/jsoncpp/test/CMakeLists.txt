find_package( PythonInterp )
if( PYTHON_EXECUTABLE )
  # There is an output regression comparison in the Python test runner, so
  # use it.
  add_test( NAME ReaderWriterTests
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${PYTHON_EXECUTABLE} runjsontests.py $<TARGET_FILE:jsontestrunner>
    )
endif()

add_test( NAME AllUnitTests
  COMMAND $<TARGET_FILE:test_lib_json>
  )
