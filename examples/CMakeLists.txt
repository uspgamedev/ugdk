project(ugdk-examples)

add_subdirectory(01)
add_subdirectory(02)
add_subdirectory(text)
if(ANDROID)
    add_subdirectory(android)
endif(ANDROID)