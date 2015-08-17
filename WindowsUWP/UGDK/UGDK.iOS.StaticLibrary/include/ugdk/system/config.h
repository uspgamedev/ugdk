#ifndef UGDK_COMMON_CONFIG_H_
#define UGDK_COMMON_CONFIG_H_

// Version
#define UGDK_VERSION_MAJOR 0
#define UGDK_VERSION_MINOR 5
#define UGDK_VERSION_PATCH 0

#define UGDK_BIGVERSION "0.5"
#define UGDK_VERSION "0.5.0"

///
//#cmakedefine CHECKMAC
//#cmakedefine UGDK_INSTALL_LOCATION "@UGDK_INSTALL_LOCATION@"
///

//#cmakedefine UGDK_SWIG_ENABLED
//#cmakedefine UGDK_LUA_ENABLED
//#cmakedefine UGDK_PYTHON_ENABLED
//#cmakedefine UGDK_3D_ENABLED
#define UGDK_ENABLED(X) UGDK_##X##_ENABLED

//#cmakedefine UGDK_INSTALL_LOCATION "@UGDK_INSTALL_LOCATION@"

//#cmakedefine OGRE_PLUGIN_DIR_REL "@OGRE_PLUGIN_DIR_REL@"
//#cmakedefine OGRE_PLUGIN_DIR_DBG "@OGRE_PLUGIN_DIR_DBG@"

#ifdef ANDROID
#include <cstdio>
#include <jni.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "UGDK"
#define printf(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define fprintf(A, ...) printf(__VA_ARGS__)

#endif

#endif // UGDK_COMMON_CONFIG_H_
