
#ifndef MODULE_LUAPROXY_H_
#define MODULE_LUAPROXY_H_

#include <ugdk/script/languages/lua/header.h>
#include <ugdk/script/languages/lua/state.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/virtualdata.h>

namespace ugdk {
namespace script {
namespace lua {

static int ClassGet(lua_State* L) {
    /*  there should be 2 params passed in
      (1) userdata (not the meta table)
      (2) string name of the attribute
    */
  assert(lua_isuserdata(L,-2) || lua_istable(L,-2));  /* just in case */
  lua_getmetatable(L,-2);    /* get the meta table */
  assert(lua_istable(L,-1));  /* just in case */
  SWIG_Lua_get_table(L,".get"); /* find the .get table */
  assert(lua_istable(L,-1));  /* just in case */
  /* look for the key in the .get table */
  lua_pushvalue(L,2);  /* key */
  lua_rawget(L,-2);
  lua_remove(L,-2); /* stack tidy, remove .get table */
  if (lua_iscfunction(L,-1))
  {  /* found it so call the fn & return its value */
    lua_pushvalue(L,1);  /* the userdata */
    lua_call(L,1,1);  /* 1 value in (userdata),1 out (result) */
    lua_remove(L,-2); /* stack tidy, remove metatable */
    return 1;
  }
  lua_pop(L,1);  /* remove whatever was there */
  /* ok, so try the .fn table */
  SWIG_Lua_get_table(L,".fn"); /* find the .get table */
  assert(lua_istable(L,-1));  /* just in case */
  lua_pushvalue(L,2);  /* key */
  lua_rawget(L,-2);  /* look for the fn */
  lua_remove(L,-2); /* stack tidy, remove .fn table */
  if (lua_isfunction(L,-1)) /* note: if its a C function or lua function */
  {  /* found it so return the fn & let lua call it */
    lua_remove(L,-2); /* stack tidy, remove metatable */
    return 1;
  }
  lua_pop(L,1);  /* remove whatever was there */
  /* NEW: looks for the __getitem() fn
  this is a user provided get fn */
  SWIG_Lua_get_table(L,"__getitem"); /* find the __getitem fn */
  if (lua_iscfunction(L,-1))  /* if its there */
  {  /* found it so call the fn & return its value */
    lua_pushvalue(L,1);  /* the userdata */
    lua_pushvalue(L,2);  /* the parameter */
    lua_call(L,2,1);  /* 2 value in (userdata),1 out (result) */
    lua_remove(L,-2); /* stack tidy, remove metatable */
    return 1;
  }
  return 0;  /* sorry not known */
}

/* the class.set method, performs the lookup of class attributes */
static int  ClassSet(lua_State* L) {
/*  there should be 3 params passed in
  (1) table (not the meta table)
  (2) string name of the attribute
  (3) any for the new value
printf("SWIG_Lua_class_set %p(%s) '%s' %p(%s)\n",
      lua_topointer(L,1),lua_typename(L,lua_type(L,1)),
      lua_tostring(L,2),
      lua_topointer(L,3),lua_typename(L,lua_type(L,3)));*/

  assert(lua_isuserdata(L,1) || lua_istable(L,1));  /* just in case */
  lua_getmetatable(L,1);    /* get the meta table */
  assert(lua_istable(L,-1));  /* just in case */

  SWIG_Lua_get_table(L,".set"); /* find the .set table */
  if (lua_istable(L,-1))
  {
    /* look for the key in the .set table */
    lua_pushvalue(L,2);  /* key */
    lua_rawget(L,-2);
    if (lua_iscfunction(L,-1))
    {  /* found it so call the fn & return its value */
      lua_pushvalue(L,1);  /* userdata */
      lua_pushvalue(L,3);  /* value */
      lua_call(L,2,0);
      return 0;
    }
    lua_pop(L,1);  /* remove the value */
  }
  lua_pop(L,1);  /* remove the value .set table */
  /* NEW: looks for the __setitem() fn
  this is a user provided set fn */
  SWIG_Lua_get_table(L,"__setitem"); /* find the fn */
  if (lua_iscfunction(L,-1))  /* if its there */
  {  /* found it so call the fn & return its value */
    lua_pushvalue(L,1);  /* the userdata */
    lua_pushvalue(L,2);  /* the parameter */
    lua_pushvalue(L,3);  /* the value */
    lua_call(L,3,0);  /* 3 values in ,0 out */
    lua_remove(L,-2); /* stack tidy, remove metatable */
    return 1;
  }
  // final case... just put it in the goddamn table, dude.
  lua_settop(L, 3); // go back to original stack
  if (lua_istable(L, 1))
      lua_rawset(L, 1);
  return 0;
}

static void ExportMetamethods (lua_State *L) {
    lua_getfield(L, LUA_GLOBALSINDEX, "UGDK_proxymethods");
    if (lua_istable(L,-1)) // someone already made it
        return;
    lua_createtable(L, 0, 2);
    lua_pushcclosure(L, ClassGet, 0);
    lua_setfield(L, -2, "getter");
    lua_pushcclosure(L, ClassSet, 0);
    lua_setfield(L, -2, "setter");
    lua_setfield(L, LUA_GLOBALSINDEX, "UGDK_proxymethods");
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* MODULE_LUAPROXY_H_ */
