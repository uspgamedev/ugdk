
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

template <class T>
inline int CreateUserdataProxy (lua_State *L) {
    State L_(L);
    LuaWrapper* wrapper =
        static_cast<LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
    LuaData *data = wrapper->NewLuaData();
    VirtualObj proxy_obj = VirtualObj(VirtualData::Ptr(data));
    T *proxy = new T(proxy_obj);

    L_.pushudata(proxy);
    L_.newtable();
    L_.pushvalue(-1);
    L_.setfield(-2, "__index");
    L_.pushvalue(-1);
    L_.setfield(-2, "__newindex");
    L_.setmetatable(-2);
    return 1; // returns userdata now proxied
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* MODULE_LUAPROXY_H_ */
