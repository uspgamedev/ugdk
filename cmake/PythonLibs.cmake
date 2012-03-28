
find_package (PythonLibs 2.7)

if (PYTHONLIBS_FOUND)

  include_directories (${PYTHON_INCLUDE_DIRS})
  ugdk_add_scriptlang (python)

  set (UGDK_SRC ${UGDK_SRC} ${UGDK_PYTHON_SRC})
  set (PYTHON_FINAL_LIB ${PYTHON_LIBRARIES})
  # Check if we have the debug library to link with on debug. If not, we'll
  # just use the nondebug one.
  if (${BUILD_TYPE} STREQUAL DEBUG AND PYTHON_DEBUG_LIBRARY)
    set (PYTHON_FINAL_LIB ${PYTHON_DEBUG_LIBRARY})
  endif (${BUILD_TYPE} STREQUAL DEBUG AND PYTHON_DEBUG_LIBRARY)
  set (UGDK_LIBRARIES ${UGDK_LIBRARIES} ${PYTHON_FINAL_LIB})
  unset (PYTHON_FINAL_LIB)

  message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)

  message ("Could NOT find Lua5.1. No Lua5.1 script modules will be compiled.")

endif (PYTHONLIBS_FOUND)

