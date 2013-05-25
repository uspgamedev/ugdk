
find_package (PythonLibs 2.6)

if (PYTHONLIBS_FOUND)
    include_directories (${PYTHON_INCLUDE_DIRS})
    ugdk_add_scriptlang (python)
    
    set(UGDK_PYTHON_ENABLED True)

    LIST(APPEND UGDK_SRC ${UGDK_PYTHON_SRC}) 
    if(PYTHON_DEBUG_LIBRARY)
        LIST(APPEND UGDK_LIBRARIES optimized;${PYTHON_LIBRARY_RELEASE};debug;${PYTHON_DEBUG_LIBRARY})
    else()
        LIST(APPEND UGDK_LIBRARIES ${PYTHON_LIBRARIES})
    endif()
    message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)
    message ("Could NOT find Python. No Python script modules will be compiled.")
endif (PYTHONLIBS_FOUND)

