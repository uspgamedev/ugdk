
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
  
    set (UGDK_PYTHON_SCRIPTS_INSTALL_LOCATION share/ugdk/${FRAMEWORK_BIGVERSION}/python)

    message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)
    message ("Could NOT find Python. No Python script modules will be compiled.")
endif (PYTHONLIBS_FOUND)

