

include(${PROJECT_SOURCE_DIR}/modules/ugdk-script/lua_src_list.cmake)
if(NOT UGDK_LUA_SRC)
    message(FATAL_ERROR "${PROJECT_SOURCE_DIR}/modules/ugdk-script/lua_src_list.cmake did not define UGDK_LUA_SRC.")
endif()

add_subdirectory(externals/lua-5.1)

set(UGDK_LUA_ENABLED True)
LIST(APPEND UGDK_SRC       ${UGDK_LUA_SRC})
LIST(APPEND UGDK_LIBRARIES lua)
ugdk_add_scriptlang (lua)

message ("-- Lua5.1 modules added.")

