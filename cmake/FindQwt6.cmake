# Find the Qwt 6.x includes and library, either the version linked to Qt3 or the version linked to Qt4
#
# On Windows it makes these assumptions:
#    - the Qwt DLL is where the other DLLs for Qt are (QT_DIR\bin) or in the path
#    - the Qwt .h files are in QT_DIR\include\Qwt or in the path
#    - the Qwt .lib is where the other LIBs for Qt are (QT_DIR\lib) or in the path
#
# Qwt6_INCLUDE_DIR - where to find qwt.h if Qwt
# Qwt6_LIBRARY - The Qwt6 library linked against Qt4 (if it exists)
# Qwt6_FOUND - Set to TRUE if Qwt6 was found (linked either to Qt3 or Qt4)

# Copyright (c) 2007, Pau Garcia i Quiles, <pgquiles@elpauer.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Condition is "(A OR B) AND C", CMake does not support parentheses but it evaluates left to right
IF(Qwt6_LIBRARY AND Qwt6_INCLUDE_DIR)
    SET(Qwt6_FIND_QUIETLY TRUE)
ENDIF(Qwt6_LIBRARY AND Qwt6_INCLUDE_DIR)

IF(NOT QT4_FOUND)
  FIND_PACKAGE( Qt4 REQUIRED QUIET )
ENDIF(NOT QT4_FOUND)

IF( QT4_FOUND )
  # Is Qwt6 installed? Look for header files
  FIND_PATH( Qwt6_INCLUDE_DIR qwt.h
               PATHS ${QT_INCLUDE_DIR} /usr/local/qwt6/include /usr/include/qwt6 ${QWT_DIR}/include ${QWT_DIR}/src
               PATH_SUFFIXES qwt qwt6 qwt-qt4 qwt6-qt4 include qwt/include qwt6/include qwt-qt4/include qwt6-qt4/include ENV PATH)

  # Find Qwt version
  IF( Qwt6_INCLUDE_DIR )
    FILE( READ ${Qwt6_INCLUDE_DIR}/qwt_global.h QWT_GLOBAL_H )
    STRING( REGEX MATCH "#define *QWT_VERSION *(0x06*)" QWT_IS_VERSION_6 ${QWT_GLOBAL_H})

    IF( QWT_IS_VERSION_6 )
      STRING(REGEX REPLACE ".*#define[\\t\\ ]+QWT_VERSION_STR[\\t\\ ]+\"([0-9]+\\.[0-9]+\\.[0-9]+)\".*" "\\1" Qwt_VERSION "${QWT_GLOBAL_H}")

      # Find Qwt6 library linked to Qt4
      IF (UNIX)
        # General purpose method
        FIND_LIBRARY( Qwt6_Qt4_TENTATIVE_LIBRARY NAMES qwt6-qt4 qwt-qt4 qwt6 qwt PATHS /usr/local/qwt/lib /usr/local/lib /usr/lib )
        IF( UNIX AND NOT CYGWIN AND NOT APPLE)
          IF( Qwt6_Qt4_TENTATIVE_LIBRARY )
            #MESSAGE("Qwt6_Qt4_TENTATIVE_LIBRARY = ${Qwt6_Qt4_TENTATIVE_LIBRARY}")
            EXECUTE_PROCESS( COMMAND "ldd" ${Qwt6_Qt4_TENTATIVE_LIBRARY} OUTPUT_VARIABLE Qwt_Qt4_LIBRARIES_LINKED_TO )
            STRING( REGEX MATCH "QtCore" Qwt6_IS_LINKED_TO_Qt4 ${Qwt_Qt4_LIBRARIES_LINKED_TO})
            IF( Qwt6_IS_LINKED_TO_Qt4 )
              SET( Qwt6_LIBRARY ${Qwt6_Qt4_TENTATIVE_LIBRARY} )
              SET( Qwt6_FOUND TRUE )
              IF (NOT Qwt6_FIND_QUIETLY)
                MESSAGE( STATUS "Found Qwt: ${Qwt6_Qt4_LIBRARY}" )
              ENDIF (NOT Qwt6_FIND_QUIETLY)
            ENDIF( Qwt6_IS_LINKED_TO_Qt4 )
          ENDIF( Qwt6_Qt4_TENTATIVE_LIBRARY )
        ELSE( UNIX AND NOT CYGWIN AND NOT APPLE)
        # Assumes qwt.dll is in the Qt dir
          SET( Qwt6_LIBRARY ${Qwt6_Qt4_TENTATIVE_LIBRARY} )
          SET( Qwt6_FOUND TRUE )
          IF (NOT Qwt6_FIND_QUIETLY)
            MESSAGE( STATUS "Found Qwt version ${Qwt_VERSION} linked to Qt4" )
          ENDIF (NOT Qwt6_FIND_QUIETLY)
        ENDIF( UNIX AND NOT CYGWIN AND NOT APPLE)

      ELSE (UNIX)
        IF (MSVC)
          # Ad-hoc windows method for MSVC
          # At this time assumes Qt4 version
          FIND_LIBRARY(QWT6_LIBRARY_RELEASE
            NAMES qwt.lib
            PATHS ${QWT_DIR}/lib)
          FIND_LIBRARY(QWT6_LIBRARY_DEBUG
            NAMES qwtd.lib
            PATHS ${QWT_DIR}/lib)
          win32_tune_libs_names (QWT6)

          SET (Qwt6_LIBRARY ${QWT6_LIBRARIES})

          IF (QWT6_LIBRARIES)
            SET( Qwt6_FOUND TRUE )
            IF (NOT Qwt6_FIND_QUIETLY)
              MESSAGE( STATUS "Found Qwt version ${Qwt_VERSION} linked to Qt4" )
            ENDIF (NOT Qwt6_FIND_QUIETLY)
          ENDIF (QWT6_LIBRARIES)
        ELSE (MSVC)
          FIND_LIBRARY(QWT6_LIBRARY
            NAMES libqwt.a
            PATHS ${QWT_DIR}/lib)

          IF (QWT6_LIBRARIES)
            SET( Qwt6_FOUND TRUE )
            IF (NOT Qwt6_FIND_QUIETLY)
              MESSAGE( STATUS "Found Qwt version ${Qwt_VERSION} linked to Qt4" )
            ENDIF (NOT Qwt6_FIND_QUIETLY)
          ENDIF (QWT6_LIBRARIES)
        ENDIF (MSVC)
      ENDIF (UNIX)

    ENDIF( QWT_IS_VERSION_6 )

    MARK_AS_ADVANCED( Qwt6_INCLUDE_DIR Qwt6_LIBRARY )
  ENDIF( Qwt6_INCLUDE_DIR )

     IF (NOT Qwt6_FOUND AND Qwt6_FIND_REQUIRED)
          MESSAGE(FATAL_ERROR "Could not find Qwt 6.x")
     ENDIF (NOT Qwt6_FOUND AND Qwt6_FIND_REQUIRED)

ENDIF( QT4_FOUND )
