cmake_minimum_required(VERSION 3.20)

# set the project name
project(TofReco)

# find ROOT package
find_package(ROOT REQUIRED COMPONENTS RIO Net)

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
set( SAMPIC_SRC_FILES
    ${SAMPIC_FOLDER}/sources_lib/SAMPIC_256Ch_lib.c
    ${SAMPIC_FOLDER}/sources_lib/SAMPIC_256Ch_hardware_core.c
    )
add_definitions( -D USE_OFFICIAL_SAMPIC_REPO )
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
message (STATUS "Generating dictionary...")
ROOT_GENERATE_DICTIONARY(TofObjectsDict MODULE TofObjectsLinkDef LINKDEF ${HEADERS})
set_source_files_properties(TofObjectsLinkDef.cxx PROPERTIES GENERATED TRUE)
set(GENERATED_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/TofObjectsDict.cxx)
message(STATUS "Generated dictionary: ${GENERATED_DICTIONARY}")

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
message(STATUS "Creating executables...")
add_executable(GenerateRootFile ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/GenerateRootFile.cpp)
add_executable(ReadRootFile ${SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/ReadRootFile.cpp)
message(STATUS "Created executables.")

# link the ROOT libraries
message(STATUS "Linking ROOT libraries...")
target_link_libraries(GenerateRootFile ${ROOT_LIBRARIES})
target_link_libraries(ReadRootFile ${ROOT_LIBRARIES})
message(STATUS "Linked ROOT libraries.")