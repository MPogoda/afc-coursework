project( afc )

cmake_minimum_required( VERSION 2.8.6 )
set( CMAKE_AUTOMOC true )

set( CMAKE_CXX_FLAGS "-std=c++11" )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
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
