
find_package (PythonLibs 2.7)

if (PYTHONLIBS_FOUND)

  include_directories (${PYTHON_INCLUDE_DIRS})
  LIST(APPEND UGDK_SRC              ${UGDK_PYTHON_SRC})
  ugdk_add_scriptlang (python)

  message ("-- Python modules added.")
    
else (PYTHONLIBS_FOUND)

  message ("Could NOT find Python. No Python script modules will be compiled.")

endif (PYTHONLIBS_FOUND)

