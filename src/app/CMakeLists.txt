
cmessage( STATUS "Creating readTofData app..." )
add_executable( readTofData ${CMAKE_CURRENT_SOURCE_DIR}/src/readTofData.cpp )
target_link_libraries( readTofData TofRecoClasses )
install( TARGETS readTofData DESTINATION bin )

cmessage( STATUS "Creating compareTofRuns app..." )
add_executable( compareTofRuns ${CMAKE_CURRENT_SOURCE_DIR}/src/compareTofRuns.cpp )
target_link_libraries( compareTofRuns TofRecoClasses )
install( TARGETS compareTofRuns DESTINATION bin )

cmessage( STATUS "Creating printTofData app..." )
add_executable( printTofData ${CMAKE_CURRENT_SOURCE_DIR}/src/printTofData.cpp )
target_link_libraries( printTofData TofRecoClasses )
install( TARGETS printTofData compareTofRuns DESTINATION bin )

