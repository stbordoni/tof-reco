cmake_minimum_required(VERSION 3.20)

# set the project name
project(TofReco)

# Options to be triggerd with cmake -D OPTION_NAME=Value
option( BUILD_ROOT_DICTIONARY "Generate and build ROOT dictionary" ON )


# Detecting compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  message( STATUS "Detected GCC version: ${CMAKE_CXX_COMPILER_VERSION}" )
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.8.5)
    message( FATAL_ERROR "GCC version must be at least 5.0" )
  endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" )
  message( STATUS "Detected Clang version: ${CMAKE_CXX_COMPILER_VERSION}" )
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
    message( FATAL_ERROR "Clang version must be at least 3.3" )
  endif()
else()
  message( WARNING "You are using an untested compiler." )
endif()


# find ROOT package
find_package( ROOT REQUIRED COMPONENTS RIO Net )

message( STATUS "[ROOT]: ROOT found" )
include(${ROOT_USE_FILE})
# cmessage(STATUS "[ROOT]: ROOT packages found ${ROOT_LIBRARIES}")
message( STATUS "[ROOT]: ROOT include directory: ${ROOT_INCLUDE_DIRS}")
message( STATUS "[ROOT]: ROOT C++ Flags: ${ROOT_CXX_FLAGS}")

# inc dir is $ROOTSYS/include/root
set(CMAKE_ROOTSYS ${ROOT_INCLUDE_DIRS}/..)


# CXX standard is required and must match the version ROOT was compiled with.
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Try to figure out which version of C++ was used to compile ROOT.  ROOT
# generates header files that depend on the compiler version so we will
# need to use the same version.
execute_process(COMMAND root-config --has-cxx14 COMMAND grep yes
    OUTPUT_VARIABLE ROOT_cxx14_FOUND
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --has-cxx17 COMMAND grep yes
    OUTPUT_VARIABLE ROOT_cxx17_FOUND
    OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND root-config --has-cxx20 COMMAND grep yes
    OUTPUT_VARIABLE ROOT_cxx20_FOUND
    OUTPUT_STRIP_TRAILING_WHITESPACE)

# Explicitly set the compiler version so that it will match the
# compiler that was used to compile ROOT.  Recent ROOT documentation
# explicitly notes that the appliation needs to use the same C++
# standard as ROOT.
if( ROOT_cxx14_FOUND )
  message(STATUS "ROOT compiled with C++14")
  set(CMAKE_CXX_STANDARD 14)
elseif( ROOT_cxx17_FOUND )
  message(STATUS "ROOT compiled with C++17")
  set(CMAKE_CXX_STANDARD 17)
elseif( ROOT_cxx20_FOUND )
  message(STATUS "ROOT compiled with C++20")
  set(CMAKE_CXX_STANDARD 20)
else()
  message(WARNING "ROOT C++ standard not set, use ROOT minimum (C++14)")
  set(CMAKE_CXX_STANDARD 14)
endif()


message(STATUS "The current source directory is ${CMAKE_CURRENT_SOURCE_DIR}")

# link submodules
message(STATUS "Adding submodules to the project...")

###################################
# Sampic
###################################

# cmessage(STATUS "Using official sampic repo.")
set( SAMPIC_FOLDER ${CMAKE_SOURCE_DIR}/submodules/sampic_256ch_linux )
# message(STATUS "SAMPIC_FOLDER = ${SAMPIC_FOLDER}")
set( SAMPIC_INCLUDE_DIR ${SAMPIC_FOLDER}/include_lib )
include_directories( ${SAMPIC_INCLUDE_DIR} )
add_definitions( -D __LINUX_BUILD )

###################################
# Add the TOF common lib
###################################
include_directories( ${CMAKE_SOURCE_DIR}/submodules/tof-data-format/include )

message(STATUS "Added submodules.")

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
if( BUILD_ROOT_DICTIONARY )
  message (STATUS "Generating dictionary...")
  ROOT_GENERATE_DICTIONARY(TofObjectsDict MODULE TofObjectsLinkDef LINKDEF ${HEADERS})
  set_source_files_properties(TofObjectsLinkDef.cxx PROPERTIES GENERATED TRUE)
  set(GENERATED_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/TofObjectsDict.cxx)
  message(STATUS "Generated dictionary: ${GENERATED_DICTIONARY}")
endif()


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

message( STATUS "Defining lib: TofRecoClasses" )

add_library(
    TofRecoClasses
    STATIC
    ${SOURCES}
)

target_link_libraries(
    TofRecoClasses
    PUBLIC
    ${ROOT_LIBRARIES}
)



# create the executables
message(STATUS "Creating executables...")
add_executable(GenerateRootFile ${GENERATED_DICTIONARY} ${CMAKE_CURRENT_SOURCE_DIR}/GenerateRootFile.cpp)
add_executable(ReadRootFile ${GENERATED_DICTIONARY} ${CMAKE_CURRENT_SOURCE_DIR}/ReadRootFile.cpp)
message(STATUS "Created executables.")

# link the ROOT libraries
message(STATUS "Linking ROOT libraries...")
target_link_libraries(GenerateRootFile TofRecoClasses)
target_link_libraries(ReadRootFile TofRecoClasses)
message(STATUS "Linked ROOT libraries.")