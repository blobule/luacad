

FIND_PATH(OPENCSG_INCLUDE_DIR opencsg.h)

FIND_LIBRARY(OPENCSG_LIBRARY opencsg)
	
IF(OPENCSG_INCLUDE_DIR AND OPENCSG_LIBRARY)
    SET( OPENCSG_FOUND "YES" )
    SET( OPENCSG_LIBS ${OPENCSG_LIBRARY} )
ELSE()
    SET( OPENCSG_FOUND "NO" )
ENDIF()

IF(OPENCSG_FOUND)
  IF(NOT OPENCSG_FIND_QUIETLY)
      MESSAGE(STATUS "Found OpenCSG: ${OPENCSG_LIBS}")
  ENDIF()
ELSE()
  IF(OPENCSG_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find OpenCSG")
  ENDIF()
ENDIF()

MARK_AS_ADVANCED(
  OPENCSG_INCLUDE_DIR
  OPENCSG_LIBRARY
)
