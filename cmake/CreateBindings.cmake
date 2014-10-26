
#set (CMAKE_CURRENT_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${GENERATED_DIR}")
set (CMAKE_SWIG_OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/src/generated")

find_host_package (SWIG)

if (SWIG_FOUND)
    include (${ugdk_SOURCE_DIR}/cmake/UseSWIG.cmake)

    macro (ugdk_add_scriptlang lang MODULE_LIST_VAR GENERATED_OUTPUT)
        swig_add_module (ugdk_${lang} ${lang} ${${MODULE_LIST_VAR}})
        set (TEMP_ONLY_CXX)
        foreach (it ${swig_generated_sources})
            if (${it} MATCHES ".cc$")
                set (TEMP_ONLY_CXX ${TEMP_ONLY_CXX} ${it})
            endif ()
        endforeach (it)
        set(${GENERATED_OUTPUT} ${TEMP_ONLY_CXX})
        unset (TEMP_ONLY_CXX)
    endmacro (ugdk_add_scriptlang lang)

    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/src/module")
    set(CMAKE_SWIG_FLAGS
        "-I${ugdk_SOURCE_DIR}/src"
        "-I${ugdk_SOURCE_DIR}/modules/ugdk-core/src"
        "-I${ugdk_SOURCE_DIR}/modules/ugdk-core/include"
        "-I${ugdk_SOURCE_DIR}/modules/ugdk-script/include"
        "-I${ugdk_BINARY_DIR}/include")
    if (UGDK_3D_ENABLED)
        set(CMAKE_SWIG_FLAGS "${CMAKE_SWIG_FLAGS}"
            "-I${ugdk_SOURCE_DIR}/modules/ugdk-3d/include"
            "-I${ugdk_SOURCE_DIR}/modules/ugdk-3d/src")
    else()
        set(CMAKE_SWIG_FLAGS "${CMAKE_SWIG_FLAGS}"
            "-I${ugdk_SOURCE_DIR}/modules/ugdk-2d/include"
            "-I${ugdk_SOURCE_DIR}/modules/ugdk-2d/src")
    endif()
    
    function (create_modules LANGUAGE_LIST_VAR MODULE_LIST_VAR MODULES_NAME GENERATED_OUTPUT)
        set_source_files_properties (${${MODULE_LIST_VAR}} PROPERTIES CPLUSPLUS ON)
        set_source_files_properties (${${MODULE_LIST_VAR}} PROPERTIES SWIG_FLAGS "")
        
        set(GENERATED_SRC)
        
        set(MODULES_LIST "")
        foreach(it ${${MODULE_LIST_VAR}})
            get_filename_component(val ${it} NAME_WE)
            string(REGEX REPLACE "_[23]D" "" val ${val})
            set(MODULES_LIST "${MODULES_LIST} \\\n    ACTION(LANG, ${val})")
        endforeach()
        
        set(LANGUAGES_LIST "")
        foreach(it ${${LANGUAGE_LIST_VAR}})
            string(TOUPPER ${it} itUPPER)
            string(TOLOWER ${it} itLOWER)
            ugdk_add_scriptlang (${itLOWER} ${MODULE_LIST_VAR} MODULE_GENERATED)
            list(APPEND GENERATED_SRC ${MODULE_GENERATED})
            if(UGDK_${itUPPER}_ENABLED)
                set(LANGUAGES_LIST "${LANGUAGES_LIST} \\\n    ACTION(${itUPPER})")
            endif()
        endforeach()
        
        list(APPEND GENERATED_SRC modules.cc)
        configure_file(${ugdk_SOURCE_DIR}/cmake/modules.cc.in modules.cc)
        unset(MODULES_LIST)
        unset(LANGUAGES_LIST)
                
        set(${GENERATED_OUTPUT} ${GENERATED_SRC} PARENT_SCOPE)
    endfunction ()
  
else (SWIG_FOUND)
    message ("--- SWIG not found, ignoring UGDK_CREATE_BINDINGS.")
endif (SWIG_FOUND)

