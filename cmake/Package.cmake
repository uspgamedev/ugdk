##########################################
# Package Configuration

set (CPACK_GENERATOR DEB)
set (CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set (CPACK_PACKAGE_NAME libugdk)
set (CPACK_PACKAGE_VENDOR ${GROUP_NAME})
set (CPACK_PACKAGE_CONTACT "${GROUP_NAME} <${CONTACT_MAIL}>")
set (CPACK_PACKAGE_INSTALL_REGISTRY_KEY "ugdk")
set (CPACK_PACKAGE_VERSION_MAJOR ${UGDK_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${UGDK_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${UGDK_VERSION_PATCH})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "USP Game Development Kit, a framework for developing 2d games.")
set (CPACK_DEBIAN_PACKAGE_NAME "libugdk${UGDK_BIGVERSION}")
set (CPACK_DEBIAN_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), libsdl1.2-dev, libsdl-image1.2, libsdl-mixer1.2-dev, libsdl-ttf2.0-0, lua5.1, python (>=2.6)")
set (DEBIAN_PACKAGE_BUILDS_DEPENDS "libc6 (>= 2.2), libgcc1 (>= 1:4.1.1), libstdc++6 (>= 4.4.0), libsdl1.2-dev, libsdl-image1.2-dev, libsdl-mixer1.2-dev, libsdl-ttf2.0-dev, swig2.0, lua5.1-dev, python-dev (>= 2.6), cmake")
set (CPACK_NSIS_CONTACT ${CONTACT_MAIL})
set (CPACK_NSIS_DISPLAY_NAME "USP Game Development Kit")
set (CPACK_NSIS_PACKAGE_HOMEPAGE ${HOME_URL})
set (CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set (CPACK_RPM_PACKAGE_URL ${HOME_URL})

include (CPack)

#
##########################################