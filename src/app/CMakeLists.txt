

cmessage( STATUS "Creating readTofData and compareTofRuns app..." )
add_executable( readTofData ${CMAKE_CURRENT_SOURCE_DIR}/src/readTofData.cpp )
add_executable( compareTofRuns ${CMAKE_CURRENT_SOURCE_DIR}/src/compareTofRuns.cpp )
add_executable( anaTofData ${CMAKE_CURRENT_SOURCE_DIR}/src/anaTofData.cpp )
install( TARGETS readTofData compareTofRuns anaTofData DESTINATION bin )
target_link_libraries( readTofData TofRecoClasses )
target_link_libraries( compareTofRuns TofRecoClasses )
target_link_libraries( anaTofData TofRecoClasses )


