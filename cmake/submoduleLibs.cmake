###################################
# Sampic
###################################

if( USE_OFFICIAL_SAMPIC_REPO STREQUAL "ON" )
    # cmessage(STATUS "Using official sampic repo.")
    set( SAMPIC_FOLDER ${CMAKE_SOURCE_DIR}/submodules/sampic_256ch_linux )
    set( SAMPIC_INCLUDE_DIR ${SAMPIC_FOLDER}/include_lib )
    set( SAMPIC_SRC_FILES
        ${SAMPIC_FOLDER}/sources_lib/SAMPIC_256Ch_lib.c
        ${SAMPIC_FOLDER}/sources_lib/SAMPIC_256Ch_hardware_core.c
        )
    add_definitions( -D USE_OFFICIAL_SAMPIC_REPO )
else()
    # cmessage(STATUS "Using sampic repo from linux software repo.")
    set( SAMPIC_FOLDER ${CMAKE_SOURCE_DIR}/submodules/sampic/Sampic_Linux/SAMPIC_256Ch_lib_v_2.2-0.2.1 )
    set( SAMPIC_INCLUDE_DIR ${SAMPIC_FOLDER}/include )
    set( SAMPIC_SRC_FILES
        ${SAMPIC_FOLDER}/src/SAMPIC_256Ch_lib.c
        ${SAMPIC_FOLDER}/src/SAMPIC_256Ch_hardware_core.c
        )
endif()

include_directories( ${SAMPIC_INCLUDE_DIR} )

#add_library( ftd2xx STATIC IMPORTED )
#set_target_properties( ftd2xx PROPERTIES IMPORTED_LOCATION /root/repo/tof_midas_fe/external/libftd2xx-x86_64-1.4.27/release/build/libftd2xx.a )

#link_directories( ${CMAKE_SOURCE_DIR}/external/libftd2xx-x86_64-1.4.27/release/build/ )
#link_directories( ${CMAKE_SOURCE_DIR}/submodules/sampic/Sampic_Linux/lpdevclib-install/ubuntu/lib/ )

add_library( ftd2xx SHARED IMPORTED )
set_target_properties( ftd2xx PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/submodules/sampic/Sampic_Linux/lpdevclib-install/libftd2xx.so.1.4.8 )

add_library( lpdev SHARED IMPORTED )
set_target_properties( lpdev PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/submodules/sampic/Sampic_Linux/lpdevclib-install/ubuntu/lib/liblpdev.so )

add_library( lpdevC SHARED IMPORTED )
set_target_properties( lpdevC PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/submodules/sampic/Sampic_Linux/lpdevclib-install/ubuntu/lib/liblpdevC.so )

add_definitions( -D __LINUX_BUILD )
add_library( sampic256ch STATIC ${SAMPIC_SRC_FILES} )
target_link_libraries( sampic256ch PRIVATE lpdevC lpdev ftd2xx )

install( TARGETS sampic256ch DESTINATION lib )

###################################
# Add the TOF common lib
###################################
include_directories( ${CMAKE_SOURCE_DIR}/submodules/tof-data-format/include )
