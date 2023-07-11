
cmessage( STATUS "Creating readTofData app..." )
add_executable( readTofData ${CMAKE_CURRENT_SOURCE_DIR}/src/readTofData.cpp )
target_link_libraries( readTofData TofRecoClasses )
install( TARGETS readTofData DESTINATION bin )

cmessage( STATUS "Creating compareTofRuns app..." )
add_executable( compareTofRuns ${CMAKE_CURRENT_SOURCE_DIR}/src/compareTofRuns.cpp )
target_link_libraries( compareTofRuns TofRecoClasses )
install( TARGETS compareTofRuns DESTINATION bin )

cmessage( STATUS "Creating tofPrintData app..." )
add_executable( tofPrintData ${CMAKE_CURRENT_SOURCE_DIR}/src/tofPrintData.cpp)
target_link_libraries( tofPrintData TofRecoClasses )
install( TARGETS tofPrintData compareTofRuns DESTINATION bin )

