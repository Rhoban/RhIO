file( GLOB headers json/*.h )
install( FILES
  ${headers}
  DESTINATION include/json
  COMPONENT DEVELOPMENT
  )
