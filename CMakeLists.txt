project( afc )

cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()

find_package( Qt4 REQUIRED QtGui QtCore )

include( ${QT_USE_FILE} )

set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" )
find_package( Qwt6 REQUIRED )
include_directories( ${Qwt6_INCLUDE_DIR} )

set( SRCS
  main.cxx
  main_widget.cxx
)


add_subdirectory( convex_hull )

add_executable( afc ${SRCS} )

add_dependencies( afc
  convex_hull
)

target_link_libraries( afc convex_hull ${QT_LIBRARIES} ${Qwt6_LIBRARY} )
