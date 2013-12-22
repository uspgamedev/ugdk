
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET (CHECKMAC TRUE)
endif ()

IF (UNIVERSAL)
    # Necessary for a Mac Universal Binary
    SET (CMAKE_OSX_ARCHITECTURES ppc;i386;x86_64)
    SET (CMAKE_OSX_SYSROOT /Developer/SDKs/MacOSX10.5.sdk)
    SET (MACOSX_DEPLOYMENT_TARGET 10.5)
ENDIF ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    LIST(APPEND UGDK_LINK_FLAGS "-mmacosx-version-min=10.5")
endif ()
