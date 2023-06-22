

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
set( CMAKE_CXX_STANDARD_REQUIRED ON )

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


# JSON
# NLOHMANN JSON
find_package( nlohmann_json )

find_path(NLOHMANN_JSON_INCLUDE_DIR NAMES nlohmann/json.hpp)
if (nlohmann_json_FOUND)
  cmessage( STATUS "nlohmann JSON library found: ${NLOHMANN_JSON_INCLUDE_DIR}")
  # Additional actions for when the library is found
else()
  if (NLOHMANN_JSON_INCLUDE_DIR)
    cmessage( STATUS "nlohmann JSON header found: ${NLOHMANN_JSON_INCLUDE_DIR}/nlohmann/json.hpp")
    # Additional actions for when the library is found
  else()
    cmessage( FATAL_ERROR "nlohmann JSON library not found")
    # Additional actions for when the library is not found
  endif()
endif()
include_directories( ${NLOHMANN_JSON_INCLUDE_DIR} )


