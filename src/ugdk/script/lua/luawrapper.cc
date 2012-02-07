

#include <ugdk/script/lua/luawrapper.h>

#include <ugdk/script/virtualdata.h>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/luadata.h>

#include <ugdk/script/scriptmanager.h>
#include <ugdk/util/pathmanager.h>
#include <ugdk/base/engine.h>

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
    if (data_gear_) return true;
    BootstrapGear btgear;
    do {
        if (!btgear.Initialize(modules_)) break;
        modules_.clear();
        data_gear_ = btgear.NextGear();
        if (!data_gear_) break;
        return true;
    } while(0);
    btgear.Abort();
    return false;
}

void LuaWrapper::Finalize() {
    delete data_gear_;
    data_gear_ = NULL;
}

VirtualData::Ptr LuaWrapper::NewData() {
    return VirtualData::Ptr(
            new LuaData(this, data_gear().GenerateID())
    );
}

VirtualObj LuaWrapper::LoadModule(const string& name) {
    string fullpath = PATH_MANAGER()->ResolvePath(
        "scripts/" +
        SCRIPT_MANAGER()->ConvertDottedNotationToPath(name) +
        "." + file_extension()
    );
    {
        const Constant result = data_gear_->DoFile(fullpath.c_str());
        puts("SETP 3");
        if(result != Constant::OK())
            return VirtualObj(VirtualData::Ptr()); // error
        puts("SETP 4");
        LuaData* lua_data = new LuaData(this, data_gear_->GenerateID());
        puts("SETP 5");
        //Share(&dtgear);
        puts("SETP 6");
        lua_data->RemoveFromBuffer();
        puts("SETP 7");
        //Share(NULL);
        puts("SETP 8");
        VirtualData::Ptr data(lua_data);
        puts("SETP 9");
        return VirtualObj(data);
    }
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
