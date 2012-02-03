

#include <ugdk/script/lua/luawrapper.h>

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/luadata.h>

namespace ugdk {
namespace script {
namespace lua {

using std::string;

/// Overwritten methods.

bool LuaWrapper::RegisterModule(string name, lua_CFunction init_func) {
    if (!gear_.ValidState())
        return false;
    return true;
}

bool LuaWrapper::Initialize() {
    if (!gear_.ValidState())
        return false;
    gear_.LoadLibs();
    return true;
}

void LuaWrapper::Finalize() {

}

VirtualData::Ptr LuaWrapper::NewData() {
    return VirtualData::Ptr(new LuaData(this));
}

VirtualObj LuaWrapper::LoadModule(const std::string& name) {
    return VirtualObj(NewData());
}

/// Other methods.

void* LuaWrapper::Unwrap(DataID id, const VirtualType& type) /*const*/ {
    gear_.PushData(id);
    return false;
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
