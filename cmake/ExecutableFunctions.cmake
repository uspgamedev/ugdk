if(ANDROID)
    macro(add_ugdk_executable NAME)
        add_library(${NAME} SHARED
            ${ARGN}
            ${ugdk_SOURCE_DIR}/src/android/main.cc
        )
        set_target_properties(${NAME} PROPERTIES OUTPUT_NAME ${NAME})
        target_link_libraries(${NAME} PUBLIC libugdk)
    endmacro(add_ugdk_executable)
    macro(add_ugdk_android_project TARGET NAME BUNDLE_NAME)
        set(UGDK_ANDROIDPROJECT_NAME ${NAME})
        set(UGDK_ANDROIDPROJECT_BUNDLE_NAME ${BUNDLE_NAME})
        get_property(UGDK_ANDROIDPROJECT_UGDK_NAME TARGET libugdk PROPERTY OUTPUT_NAME)
        get_property(UGDK_ANDROIDPROJECT_UGDK_NAME_DEBUG TARGET libugdk PROPERTY OUTPUT_NAME_DEBUG)
        get_property(UGDK_ANDROIDPROJECT_TARGET_NAME TARGET ${TARGET} PROPERTY OUTPUT_NAME)
        set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/android-project/libs/armeabi-v7a" 
            CACHE PATH "root for library output, set this to change where android libs are installed to" FORCE)
        
        file(GLOB_RECURSE ANDROID_TEMPLATE_SOURCE
            RELATIVE ${ugdk_SOURCE_DIR}/android/template
            "${ugdk_SOURCE_DIR}/android/template/*")
        file(COPY ${ugdk_SOURCE_DIR}/android/skeleton/
            DESTINATION ${CMAKE_BINARY_DIR}/android-project)
            
        foreach(it ${ANDROID_TEMPLATE_SOURCE})
            configure_file(${ugdk_SOURCE_DIR}/android/template/${it}
                           ${CMAKE_BINARY_DIR}/android-project/${it})
        endforeach()
        
        string(REPLACE "." "/" BUNDLE_NAME_AS_DIR "${BUNDLE_NAME}")
        configure_file("${ugdk_SOURCE_DIR}/android/UGDKActivity.java"
                       "${CMAKE_BINARY_DIR}/android-project/src/${BUNDLE_NAME_AS_DIR}")
        
    endmacro(add_ugdk_android_project)
else(ANDROID)
    macro(add_ugdk_executable NAME)
        add_executable(${NAME} ${ARGN})
        target_link_libraries(${NAME} PUBLIC libugdk)
    endmacro(add_ugdk_executable)
    macro(add_ugdk_android_project NAME BUNDLE_NAME)
    endmacro(add_ugdk_android_project)
endif(ANDROID)