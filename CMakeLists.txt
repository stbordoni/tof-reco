cmake_minimum_required(VERSION 3.20)

# set the project name
project(TofReco)


include( ${CMAKE_SOURCE_DIR}/cmake/cmessage.cmake )
include( ${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake )
include( ${CMAKE_SOURCE_DIR}/cmake/submodules.cmake )


add_subdirectory( ${CMAKE_SOURCE_DIR}/src/tofRecoClasses )
add_subdirectory( ${CMAKE_SOURCE_DIR}/src/app )
