
find_package (Lua51)

if (LUA51_FOUND)

  include_directories (${LUA_INCLUDE_DIR})
  LIST(APPEND UGDK_SRC              ${UGDK_LUA_SRC})
  LIST(APPEND UGDK_LIBRARIES        ${LUA_LIBRARIES})
  LIST(APPEND UGDK_LIBRARIES_DEBUG  ${LUA_LIBRARIES})
  ugdk_add_scriptlang (lua)

  message ("-- Lua5.1 modules added.")
    
else (LUA51_FOUND)

  message ("Could NOT find Lua5.1. No Lua5.1 script modules will be compiled.")

endif (LUA51_FOUND)

