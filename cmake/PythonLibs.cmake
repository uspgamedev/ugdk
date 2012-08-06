
find_package (PythonLibs 2.6)

if (PYTHONLIBS_FOUND)
    include_directories (${PYTHON_INCLUDE_DIRS})
    ugdk_add_scriptlang (python)

    LIST(APPEND UGDK_SRC ${UGDK_PYTHON_SRC}) 
    if(PYTHON_DEBUG_LIBRARY)
        LIST(APPEND UGDK_LIBRARIES optimized;${PYTHON_LIBRARIES};debug;${PYTHON_DEBUG_LIBRARY})
    else(PYTHON_DEBUG_LIBRARY)
        LIST(APPEND UGDK_LIBRARIES ${PYTHON_LIBRARIES})
    endif(PYTHON_DEBUG_LIBRARY)
  
    if (PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
        FILE(STRINGS "${PYTHON_INCLUDE_DIR}/patchlevel.h" python_version_str
             REGEX "^#define[ \t]+PY_VERSION[ \t]+\"[^\"]+\"")
        STRING(REGEX REPLACE "^#define[ \t]+PY_VERSION[ \t]+\"([0-9].[0-9])[^\"]*\".*" "\\1"
               PYTHON_VERSION "${python_version_str}")
        UNSET(python_version_str)
    endif (PYTHON_INCLUDE_DIR AND EXISTS "${PYTHON_INCLUDE_DIR}/patchlevel.h")
  
    set (UGDK_PYTHON_SCRIPTS_INSTALL_LOCATION share/ugdk${FRAMEWORK_BIGVERSION}/python)

    message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)
    message ("Could NOT find Python. No Python script modules will be compiled.")
endif (PYTHONLIBS_FOUND)

