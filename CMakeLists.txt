cmake_minimum_required(VERSION 3.20)

# set the project name
project(TofReco)

# find ROOT package
find_package(ROOT REQUIRED COMPONENTS RIO Net)

message(STATUS "The current source directory is ${CMAKE_CURRENT_SOURCE_DIR}")

# set the include directories

include( ${CMAKE_SOURCE_DIR}/cmake/submoduleLibs.cmake )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

# Add header files
set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TofHit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TofSignal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TofEvent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/TofRun.h
)


# generate dictionary
ROOT_GENERATE_DICTIONARY(TofObjectsDict MODULE TofObjectsLinkDef LINKDEF ${HEADERS})
set_source_files_properties(TofObjectsLinkDef.cxx PROPERTIES GENERATED TRUE)
set(GENERATED_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/TofObjectsDict.cxx)

# add the source files
set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TofEvent.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TofHit.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TofRun.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/TofSignal.cpp
  ${GENERATED_DICTIONARY}
  # ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  # ${CMAKE_CURRENT_SOURCE_DIR}/ReadHits.cpp
)

# create the executables
add_executable(GenerateRootFile ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/GenerateRootFile.cpp)
add_executable(ReadRootFile ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/ReadRootFile.cpp)

# link the ROOT libraries
target_link_libraries(GenerateRootFile ${ROOT_LIBRARIES})
target_link_libraries(ReadRootFile ${ROOT_LIBRARIES})