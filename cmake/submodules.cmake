
# link submodules
cmessage( STATUS "Adding submodules to the project..." )



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

