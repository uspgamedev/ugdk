if(ANDROID)
    macro(add_ugdk_executable NAME)
        add_library(${NAME} SHARED
            ${ARGN}
            ${ugdk_SOURCE_DIR}/src/android/main.cc
        )
        set_target_properties(${NAME} PROPERTIES OUTPUT_NAME ${NAME})
        target_link_libraries(${NAME} PUBLIC libugdk)
    endmacro(add_ugdk_executable)
else(ANDROID)
    macro(add_ugdk_executable NAME)
        add_executable(${NAME} ${ARGN})
        target_link_libraries(${NAME} PUBLIC libugdk)
    endmacro(add_ugdk_executable)
endif(ANDROID)