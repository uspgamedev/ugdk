
#ifndef UGDK_SCRIPT_LUA_LUAWRAPPER_H_
#define UGDK_SCRIPT_LUA_LUAWRAPPER_H_

#include <string>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/gear.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {
namespace lua {

class LuaWrapper: public LangWrapper {

  public:

    LuaWrapper() : LangWrapper("lua",LANG(Lua)) {}
    ~LuaWrapper() {}

    /// Overwritten methods.

    bool RegisterModule(std::string name, lua_CFunction init_func);

    bool Initialize();

    void Finalize();

    VirtualData::Ptr NewData();

    VirtualObj LoadModule(const std::string& name);

    /// Other methods.

    void* Unwrap(DataID, const VirtualType& type) /*const*/;

  private:

    Gear gear_;

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUAWRAPPER_H_ */
