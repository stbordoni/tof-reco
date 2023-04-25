cmake_minimum_required(VERSION 3.20)

# set the project name
project(TofReco)

# find ROOT package
find_package(ROOT REQUIRED COMPONENTS RIO Net)

message(STATUS "The current source directory is ${CMAKE_CURRENT_SOURCE_DIR}")

# set the include directories
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
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# create the executable
add_executable(TofApp ${SOURCES})

# link the ROOT libraries
target_link_libraries(TofApp ${ROOT_LIBRARIES})


# cmake_minimum_required(VERSION 3.22)

# # set the project name
# project(TofReco)

# # find ROOT package
# find_package(ROOT REQUIRED COMPONENTS RIO Net)

# # set the include directories
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${ROOT_INCLUDE_DIRS})

# # Add header files
# set(HEADERS
#     include/TofHit.h
#     include/TofSignal.h
#     include/TofEvent.h
#     include/TofRun.h
# )

# # generate dictionary
# ROOT_GENERATE_DICTIONARY(build/TofObjectsDict ${HEADERS} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/TofObjectsLinkDef.h)

# # add the source files
# set(SOURCES
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/TofEvent.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/TofHit.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/TofRun.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/TofSignal.cpp
#   ${CMAKE_CURRENT_SOURCE_DIR}/build/TofObjectsLinkDef.cxx
#   ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
# )

# # create the executable
# add_executable(TofApp ${SOURCES})

# # link the ROOT libraries
# target_link_libraries(TofApp ${ROOT_LIBRARIES})



# # cmake_minimum_required(VERSION 3.10)

# # project(TofReco)

# # # Set up ROOT
# # find_package(ROOT REQUIRED)
# # include(${ROOT_USE_FILE})

# # # Set up the TofObjects library
# # file(GLOB TofObjectsSources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# # file(GLOB TofObjectsHeaders ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
# # add_library(TofObjects SHARED ${TofObjectsSources})
# # target_include_directories(TofObjects PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
# # target_link_libraries(TofObjects PUBLIC ${ROOT_LIBRARIES})

# # # Set up the dictionary
# # ROOT_GENERATE_DICTIONARY(TofObjectsDict ${TofObjectsHeaders} LINKDEF ${CMAKE_CURRENT_SOURCE_DIR}/include/TofObjectsLinkDef.h)
# # target_sources(TofObjects PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/TofObjectsDict.cxx)
# # target_include_directories(TofObjects PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# # # Set up the executable
# # add_executable(TofApp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
# # target_link_libraries(TofApp PUBLIC TofObjects ${ROOT_LIBRARIES})

# # # Set the output directory
# # set_target_properties(TofApp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)


# # # cmake_minimum_required(VERSION 3.10)

# # # project(TofApp)

# # # # set C++ standard
# # # set(CMAKE_CXX_STANDARD 17)

# # # # set ROOT
# # # find_package(ROOT REQUIRED)
# # # include(${ROOT_USE_FILE})

# # # # Add header files
# # # set(HEADERS
# # #     include/TofHit.h
# # #     include/TofSignal.h
# # #     include/TofEvent.h
# # #     include/TofRun.h
# # # )

# # # # generate dictionary for classes
# # # ROOT_GENERATE_DICTIONARY(build/TofObjectsDict $(addprefix ../,${HEADERS}) LINKDEF include/TofObjects.h)

# # # # add source files
# # # set(SOURCES
# # #     src/TofEvent.cpp
# # #     src/TofHit.cpp
# # #     src/TofRun.cpp
# # #     src/TofSignal.cpp
# # #     main.cpp
# # #     build/TofObjectsDict.cxx
# # # )

# # # # add include directories
# # # include_directories(include)

# # # # create executable
# # # add_executable(TofApp ${SOURCES})

# # # # link ROOT libraries
# # # target_link_libraries(TofApp ${ROOT_LIBRARIES})
