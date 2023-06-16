

cmessage( STATUS "Creating readTofData app..." )
add_executable(
    readTofData
    ${CMAKE_CURRENT_SOURCE_DIR}/src/readTofData.cpp
)
target_link_libraries( readTofData TofRecoClasses )




