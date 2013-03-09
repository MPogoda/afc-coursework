project( convex_hull )
cmake_minimum_required( VERSION 2.8.6 )

if ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
  add_definitions( -std=c++11 )
elseif ( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_definitions( -std=c++0x )
endif()


set( CH_sources convex_hull.cxx )

add_library( convex_hull STATIC ${CH_sources})

