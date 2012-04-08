
find_package (Lua51)

if (LUA51_FOUND)

  include_directories (${LUA_INCLUDE_DIR})
  set (UGDK_SRC ${UGDK_SRC} ${UGDK_LUA_SRC})
  set (UGDK_LIBRARIES       ${UGDK_LIBRARIES} ${LUA_LIBRARIES})
  set (UGDK_LIBRARIES_DEBUG ${UGDK_LIBRARIES} ${LUA_LIBRARIES})
  ugdk_add_scriptlang (lua)

  message ("-- Lua5.1 modules added.")
    
else (LUA51_FOUND)

  message ("Could NOT find Lua5.1. No Lua5.1 script modules will be compiled.")

endif (LUA51_FOUND)

