
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
  
  set (CMAKE_CURRENT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${MODULE_DIR}")
  set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")

else (SWIG_FOUND)

  message (WARNING "Could NOT find SWIG. No script modules will be compiled.")

endif (SWIG_FOUND)

