
set (MODULE_DIR "${SRC_DIR}/module")
set (GENERATED_DIR "${SRC_DIR}/generated")
set (GENERATED_SRC "")

# Does this work everywhere?
find_package (SWIG)

if (SWIG_FOUND)

  include (cmake/UseSWIG.cmake)
  
  # Is MODULE_SRC defined?
  if (NOT MODULE_SRC)
      message (FATAL_ERROR "Variable MODULE_SRC not defined! Please do so in the file src/module_list.cmake!")
  endif (NOT MODULE_SRC)
  
  set_source_files_properties (${MODULE_SRC} PROPERTIES CPLUSPLUS ON)
  set_source_files_properties (${MODULE_SRC} PROPERTIES SWIG_FLAGS "")
  
  swig_add_module (ugdk_lua lua ${MODULE_SRC} ${FRAMEWORK_SRC})
  set (GENERATED_SRC ${GENERATED_SRC} ${swig_generated_sources})
  swig_add_module (ugdk_python python ${MODULE_SRC} ${FRAMEWORK_SRC})
  set (TEMP_ONLY_CXX)
  foreach (it ${swig_generated_sources})
    if (${it} MATCHES ".cc$")
      set (TEMP_ONLY_CXX ${TEMP_ONLY_CXX} ${it})
    endif ()
  endforeach (it)
  set (GENERATED_SRC ${GENERATED_SRC} ${TEMP_ONLY_CXX})
  unset (TEMP_ONLY_CXX)

else (SWIG_FOUND)

  message (WARNING "Could NOT find SWIG. No script modules will be compiled.")

endif (SWIG_FOUND)

