###################
# link submodules
###################

cmessage( STATUS "Adding submodules to the project..." )
set( SUBMODULE_DIR ${CMAKE_SOURCE_DIR}/submodules )


# Function to make sure if a given submodule is present:
function( checkSubmodule )
  list( GET ARGV 0 SELECTED_SUBMODULE )
  cmessage( WARNING "Checking submodule: ${SELECTED_SUBMODULE}" )

  file( GLOB FILES_IN_DIRECTORY "${SUBMODULE_DIR}/${SELECTED_SUBMODULE}/*")

  if( FILES_IN_DIRECTORY )
    cmessage( STATUS "Git submodule ${SELECTED_SUBMODULE} is present" )
  else()
    cmessage( ERROR "Git submodule ${SELECTED_SUBMODULE} is not present, please checkout: \"git submodule update --init --remote --recursive\"" )
    cmessage( FATAL_ERROR "CMake fatal error." )
  endif()
endfunction( checkSubmodule )



###################################
# Sampic
###################################
cmessage(STATUS "Looking for SAMPIC submodule...")
checkSubmodule( sampic_256ch_linux )
set( SAMPIC_FOLDER ${SUBMODULE_DIR}/sampic_256ch_linux )
set( SAMPIC_INCLUDE_DIR ${SAMPIC_FOLDER}/include_lib )
include_directories( ${SAMPIC_INCLUDE_DIR} )
add_definitions( -D __LINUX_BUILD )


###################################
# C++ generic toolbox
###################################
cmessage( STATUS "Looking for cpp-generic-toolbox submodule..." )
checkSubmodule( cpp-generic-toolbox )
include_directories( ${SUBMODULE_DIR}/cpp-generic-toolbox/include )
add_definitions( -D PROGRESS_BAR_FILL_TAG="\\\"T2K\#"\\\" )
add_definitions( -D PROGRESS_BAR_ENABLE_RAINBOW=1 )


###################################
# Simple C++ logger
###################################
cmessage( STATUS "Looking for simple-cpp-logger submodule..." )
checkSubmodule( simple-cpp-logger )
include_directories( ${SUBMODULE_DIR}/simple-cpp-logger/include )
add_definitions( -D LOGGER_MAX_LOG_LEVEL_PRINTED=6 )
add_definitions( -D LOGGER_PREFIX_LEVEL=3 )
add_definitions( -D LOGGER_TIME_FORMAT="\\\"%d/%m/%Y %H:%M:%S"\\\" )
add_definitions( -D LOGGER_ENABLE_COLORS=1 )
add_definitions( -D LOGGER_ENABLE_COLORS_ON_USER_HEADER=1 )
add_definitions( -D LOGGER_PREFIX_FORMAT="\\\"{TIME} {USER_HEADER} {FILELINE}"\\\" )


###################################
# Simple C++ cmd line parser
###################################
cmessage( STATUS "Looking for simple-cpp-cmd-line-parser submodule..." )
checkSubmodule( simple-cpp-cmd-line-parser )
include_directories( ${SUBMODULE_DIR}/simple-cpp-cmd-line-parser/include )


###################################
# Simple C++ cmd line parser
###################################
cmessage( STATUS "Looking for simple-cpp-cmd-line-parser submodule..." )
checkSubmodule( simple-cpp-cmd-line-parser )
include_directories( ${SUBMODULE_DIR}/simple-cpp-cmd-line-parser/include )


###################################
# midasio
###################################
cmessage( STATUS "Looking for midasio submodule..." )

# Depend on Zlib
cmessage( WARNING "Looking for ZLib install (MidasIO dependency)..." )
find_package(ZLIB)
if( ${ZLIB_FOUND} )
  cmessage( STATUS "ZLIB found : ${ZLIB_VERSION_STRING}")
  cmessage( STATUS "ZLIB_INCLUDE_DIRS = ${ZLIB_INCLUDE_DIRS}")
  cmessage( STATUS "ZLIB_LIBRARIES = ${ZLIB_LIBRARIES}")
else()
  cmessage( FATAL_ERROR "MidasIO requires Zlib" )
endif ()

set( MIDASIO_FOLDER ${SUBMODULE_DIR}/midasio )

# Header only library
include_directories( ${MIDASIO_FOLDER}/include )

set( MIDASIO_SRC_FILES
    ${MIDASIO_FOLDER}/lz4.cxx
    ${MIDASIO_FOLDER}/lz4frame.cxx
    ${MIDASIO_FOLDER}/lz4hc.cxx
    ${MIDASIO_FOLDER}/midasio.cxx
    ${MIDASIO_FOLDER}/test_midasio.cxx
    ${MIDASIO_FOLDER}/xxhash.cxx
    )

if( USE_SHARED_LINKS )
  add_library( MidasIO SHARED ${MIDASIO_SRC_FILES})
else()
  add_library( MidasIO STATIC ${MIDASIO_SRC_FILES})
endif()

target_include_directories( MidasIO PUBLIC ${MIDASIO_FOLDER} )
target_link_libraries( MidasIO PUBLIC ${ZLIB_LIBRARIES} )



###################################
# Add the TOF common lib
###################################

