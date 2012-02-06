

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
    if (L_) return true;
    L_ = AuxLib::newstate();
    {
        BootstrapGear btgear(L_);
        if (!btgear.Initialize(modules_)) return false;
        modules_.clear();
        datatable_id_ = btgear.GenerateDatatable();
    }
    return datatable_id_ != LUA_NOREF;
}

void LuaWrapper::Finalize() {
    {
        BootstrapGear btgear(L_);
        btgear.DestroyDatatable(datatable_id_);
    }
    lua_close(L_);
    L_ = NULL;
}

VirtualData::Ptr LuaWrapper::NewData() {
    return VirtualData::Ptr(
            new LuaData(this, MakeDataGear().GenerateID())
    );
}

VirtualObj LuaWrapper::LoadModule(const string& name) {
    string fullpath = name + "." + file_extension();
    {
        DataGear dtgear(MakeDataGear());
        const Constant result = dtgear.DoFile(fullpath.c_str());
        puts("SETP 3");
        if(result != Constant::OK())
            return VirtualObj(VirtualData::Ptr()); // error
        puts("SETP 4");
        LuaData* lua_data = new LuaData(this, dtgear.GenerateID());
        puts("SETP 5");
        Share(&dtgear);
        puts("SETP 6");
        lua_data->RemoveFromBuffer();
        puts("SETP 7");
        Share(NULL);
        puts("SETP 8");
        VirtualData::Ptr data(lua_data);
        puts("SETP 9");
        return VirtualObj(data);
    }
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
