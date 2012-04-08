
find_package (PythonLibs 2.7)

if (PYTHONLIBS_FOUND)

  include_directories (${PYTHON_INCLUDE_DIRS})
  ugdk_add_scriptlang (python)

  LIST(APPEND UGDK_SRC              ${UGDK_PYTHON_SRC}) 
  LIST(APPEND UGDK_LIBRARIES        ${PYTHON_LIBRARIES})
  LIST(APPEND UGDK_LIBRARIES_DEBUG  ${PYTHON_DEBUG_LIBRARY})

  message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)

  message ("Could NOT find Lua5.1. No Lua5.1 script modules will be compiled.")

endif (PYTHONLIBS_FOUND)

