
set( LIB_NAME "TofRecoClasses" )

set( SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TofRun.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TofEvent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TofHit.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TofSignal.cpp
    )

if( USE_STATIC_LINKS )
  add_library( ${LIB_NAME} STATIC ${SRC_FILES} )
else()
  add_library( ${LIB_NAME} SHARED ${SRC_FILES} )
endif()



target_include_directories(
    ${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${LIB_NAME} PUBLIC
    MidasReader
    ${ROOT_LIBRARIES}
)

install( TARGETS ${LIB_NAME} DESTINATION lib )





