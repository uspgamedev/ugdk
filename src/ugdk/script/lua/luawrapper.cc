

#include <ugdk/script/lua/luawrapper.h>
#include <ugdk/script/virtualdata.h>
#include <ugdk/script/virtualobj.h>

namespace ugdk {
namespace script {
namespace lua {

using std::string;

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
    return VirtualData::Ptr();
}

VirtualObj LuaWrapper::LoadModule(const std::string& name) {
    return VirtualObj(NewData());
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
