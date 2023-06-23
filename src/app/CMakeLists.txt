

cmessage( STATUS "Creating readTofData and compareTofRuns app..." )
add_executable( readTofData ${CMAKE_CURRENT_SOURCE_DIR}/src/readTofData.cpp )
add_executable( compareTofRuns ${CMAKE_CURRENT_SOURCE_DIR}/src/compareTofRuns.cpp )
install( TARGETS readTofData compareTofRuns DESTINATION bin )
target_link_libraries( readTofData TofRecoClasses )
target_link_libraries( compareTofRuns TofRecoClasses )


