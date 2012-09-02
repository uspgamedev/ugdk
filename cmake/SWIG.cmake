
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")
set (CMAKE_SWIG_OUTDIR "${SRC_DIR}/generated")
set (GENERATED_SRC "")

# Does this work everywhere?
find_package (SWIG)

macro (ugdk_add_scriptlang lang)

  swig_add_module (ugdk_${lang} ${lang} ${UGDK_MODULE_SRC} ${FRAMEWORK_SRC})
  set (TEMP_ONLY_CXX)
  foreach (it ${swig_generated_sources})
    if (${it} MATCHES ".cc$")
      set (TEMP_ONLY_CXX ${TEMP_ONLY_CXX} ${it})
    endif ()
  endforeach (it)
  set (GENERATED_SRC ${GENERATED_SRC} ${TEMP_ONLY_CXX})
  unset (TEMP_ONLY_CXX)

endmacro (ugdk_add_scriptlang lang)

if (SWIG_FOUND)

    include (cmake/UseSWIG.cmake)

    # Is UGDK_MODULE_SRC defined?
    if (NOT UGDK_MODULE_SRC)
        message (FATAL_ERROR "Variable UGDK_MODULE_SRC not defined! Please do so in the file src/module_list.cmake!")
    endif (NOT UGDK_MODULE_SRC)

    set_source_files_properties (${UGDK_MODULE_SRC} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties (${UGDK_MODULE_SRC} PROPERTIES SWIG_FLAGS "")
  
    list(LENGTH UGDK_MODULE_SRC UGDK_MODULES_NUM)
    foreach(it ${UGDK_MODULE_SRC})
        get_filename_component(val ${it} NAME_WE)
        set(UGDK_MODULES_LIST "${UGDK_MODULES_LIST} \\\n    ACTION(LANG, ${val})")
    endforeach()
    
    set(UGDK_USING_SWIG True)
  
else (SWIG_FOUND)

  message (WARNING "Could NOT find SWIG. No script modules will be compiled.")

endif (SWIG_FOUND)

