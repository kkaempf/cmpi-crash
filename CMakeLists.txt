#
# CMakeLists.txt for cmpi-crash
#
PROJECT(cmpi-crash)

cmake_minimum_required(VERSION 2.4)

#
# cmake2.6: backward compatibility to cmake 2.4
#
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 OLD)
endif(COMMAND cmake_policy)
	      
# Package architecture
IF ( NOT DEFINED PACKAGE_ARCHITECTURE )
 EXECUTE_PROCESS(COMMAND "/bin/uname" "-m" OUTPUT_VARIABLE UNAME_M)
 # strip trailing newline
 STRING(REPLACE "\n" "" PACKAGE_ARCHITECTURE ${UNAME_M})
ENDIF ( NOT DEFINED PACKAGE_ARCHITECTURE )
MESSAGE(STATUS "Building for ${PACKAGE_ARCHITECTURE}" )

# Library
IF ( DEFINED LIB )
  SET ( CMPI_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/${LIB}/cmpi" )
ELSE ( DEFINED  LIB )
  IF (CMAKE_SIZEOF_VOID_P MATCHES "8")
    SET( LIB_SUFFIX "64" )
  ENDIF(CMAKE_SIZEOF_VOID_P MATCHES "8")
  SET ( CMPI_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/cmpi" )
ENDIF ( DEFINED  LIB )
MESSAGE(STATUS "Providers will be installed in ${CMPI_LIBRARY_DIR}" )

INCLUDE(${CMAKE_SOURCE_DIR}/VERSION.cmake)

SET(CMAKE_VERBOSE_MAKEFILE TRUE)
SET( PACKAGE "cmpi-crash" )
SET( VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )

SET( CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -g -Wall" )
SET( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3" )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Woverloaded-virtual" )
SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -O3" )


####################################################################
# CPACK                                                            #
####################################################################


SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A crashing CMPI provider")
SET(CPACK_PACKAGE_VENDOR "SUSE Linux Products GmbH.")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
SET(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})

SET( CPACK_GENERATOR "TBZ2")
SET( CPACK_SOURCE_GENERATOR "TBZ2")
SET( CPACK_SOURCE_PACKAGE_FILE_NAME "${PACKAGE}-${VERSION}" )

# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
SET (CPACK_SOURCE_IGNORE_FILES
"/CVS/;/.svn/;/.libs/;/.deps/;.swp$;.#;/#;/build/;/.git/;/tmp/"
"~$"
"\\\\.cvsignore$"
"Makefile\\\\.in$"
)

INCLUDE(CPack)

####################################################################
# Directories                                                      #
####################################################################

ADD_DEFINITIONS(-DCMPI_PLATFORM_LINUX_GENERIC_GNU -DCMPI_VERSION=200)

#
# crashProvider: provider .so
#

SET( NAME crashProvider )
ADD_LIBRARY( ${NAME} SHARED src/crash_provider.c)
TARGET_LINK_LIBRARIES( ${NAME} )

# INSTALL(TARGETS ${NAME} LIBRARY DESTINATION ${CMPI_LIBRARY_DIR})
