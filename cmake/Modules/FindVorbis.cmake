# Locate Vorbis
#
# This module defines the standard variables
# - VORBIS_LIBRARIES
# - VORBIS_FOUND
# - VORBIS_INCLUDE_DIRS
#

SET(VORBIS_SEARCH_PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw # Fink
    /opt/local # DarwinPorts
    /opt/csw # Blastwave
    /opt
)

FIND_PATH(VORBIS_INCLUDE_DIR
    NAMES vorbis/codec.h
    HINTS
    $ENV{VORBISDIR}
    $ENV{VORBIS_PATH}
    PATH_SUFFIXES include
    PATHS ${VORBIS_SEARCH_PATHS}
)

FIND_LIBRARY(VORBIS_LIBRARY
    NAMES vorbis libvorbis
    HINTS
    $ENV{VORBISDIR}
    $ENV{VORBIS_PATH}
    PATH_SUFFIXES lib
    PATHS ${VORBIS_SEARCH_PATHS}
)


INCLUDE(FindPackageHandleStandardArgs)

set(VORBIS_LIBRARIES    ${VORBIS_LIBRARY})
set(VORBIS_INCLUDE_DIRS ${VORBIS_INCLUDE_DIR})

FIND_PACKAGE_HANDLE_STANDARD_ARGS(VORBIS
    REQUIRED_VARS VORBIS_LIBRARIES VORBIS_INCLUDE_DIRS)

mark_as_advanced(VORBIS_LIBRARY VORBIS_INCLUDE_DIR)
