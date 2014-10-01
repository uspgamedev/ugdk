
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")
set (CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/src/generated")
set (GENERATED_SRC "")


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

find_host_package (SWIG)

if (SWIG_FOUND)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/module")
    set(CMAKE_SWIG_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/src")

    include (cmake/UseSWIG.cmake)

    set_source_files_properties (${UGDK_MODULE_SRC} PROPERTIES CPLUSPLUS ON)
    set_source_files_properties (${UGDK_MODULE_SRC} PROPERTIES SWIG_FLAGS "")
    
    set(UGDK_SWIG_ENABLED True)
  
    set(MODULES_LIST "")
    foreach(it ${UGDK_MODULE_SRC})
        get_filename_component(val ${it} NAME_WE)
        string(REGEX REPLACE "_[23]D" "" val ${val})
        set(MODULES_LIST "${MODULES_LIST} \\\n    ACTION(LANG, ${val})")
    endforeach()
    
    set(LANGUAGES_LIST "")
    foreach(it ${UGDK_LANGUAGES_LIST})
        include(cmake/languages/${it}.cmake)
        string(TOUPPER ${it} itUPPER)
        if(UGDK_${itUPPER}_ENABLED)
            set(LANGUAGES_LIST "${LANGUAGES_LIST} \\\n    ACTION(${itUPPER})")
        endif()
    endforeach()
    
    set(MODULES_NAME UGDK)
    set(GENERATED_SRC ${GENERATED_SRC} modules.cc)
    configure_file(${CMAKE_CURRENT_LIST_DIR}/modules.cc.in modules.cc)
  
else (SWIG_FOUND)
    message ("--- SWIG not found, ignoring UGDK_CREATE_BINDINGS.")
endif (SWIG_FOUND)

