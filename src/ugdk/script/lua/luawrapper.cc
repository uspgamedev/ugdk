

#include <ugdk/script/lua/luawrapper.h>

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/luadata.h>

namespace ugdk {
namespace script {
namespace lua {

using std::string;
using std::vector;

/// Overwritten methods.

bool LuaWrapper::RegisterModule(const string& name, lua_CFunction init_func) {
    if (name.empty())
        return false;
    // TODO: check if name is valid.
    modules_.push_back(Module(name, init_func));
    return true;
}

bool LuaWrapper::Initialize() {
    if (L_) return false;
    L_ = luaL_newstate();
    {
        Gear g = MakeGear();
        g.LoadLibs();
        g.PreloadModule(modules_);
        modules_.clear();
        g.CreateDatatable();
    }
    return true;
}

void LuaWrapper::Finalize() {
    lua_close(L_);
    L_ = NULL;
}

VirtualData::Ptr LuaWrapper::NewData() {
    return VirtualData::Ptr(new LuaData(this));
}

VirtualObj LuaWrapper::LoadModule(const string& name) {
    string fullpath = name + "." + file_extension();
    {
        Gear g(MakeGear());
        const Constant result = g.DoFile(fullpath.c_str());
        if(result != Constant::OK())
            return VirtualObj(VirtualData::Ptr()); // error
        LuaData* lua_data = new LuaData(this);
        Share(&g);
        lua_data->RemoveFromBuffer();
        Share(NULL);
        VirtualData::Ptr data(lua_data);
        return VirtualObj(data);
    }
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
