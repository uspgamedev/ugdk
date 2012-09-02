
#ifndef UGDK_SCRIPT_LUA_LUAWRAPPER_H_
#define UGDK_SCRIPT_LUA_LUAWRAPPER_H_

#include <string>
#include <list>
#include <vector>

#include <ugdk/script/languages/lua/defs.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {
namespace lua {

class DataGear;
class LuaData;
typedef ugdk::script::InheritableLangWrapper<lua_CFunction> LuaWrapperBase;

}

#define LUA_INIT_FUNCTION_NAME(name) luaopen_##name
#define LUA_INIT_FUNCTION_SIGNATURE(name) int LUA_INIT_FUNCTION_NAME(name)(lua_State*)
#define LUA_MODULE_NAME(name) ugdk::script::lua::NameConversion(#name)
typedef lua_CFunction LUA_inittype;

namespace lua {

class LuaWrapper: public LuaWrapperBase {

  public:

    LuaWrapper() :
        LuaWrapperBase("lua",LANG(Lua), "Lua"),
        data_gear_(NULL) {}
    ~LuaWrapper() {
        if (data_gear_) Finalize();
    }

    /// Overwritten methods.

    bool Initialize();

    void Finalize();

    VirtualData::Ptr NewData();

    LuaData* NewLuaData();

    void ExecuteCode(const std::string& code);

    VirtualObj LoadModule(const std::string& name);

    /// Other methods.

    VirtualData::Ptr OperateBuffer(const DataID operand_id, lua_CFunction op);

    DataGear& data_gear() { return *data_gear_; }

    void Share(DataGear* gear) {
        data_gear_ = gear;
    }

    DataGear* shared_gear() {
        return data_gear_;
    }

    void AddToBuffer(DataID id) {
        buffer_.push_back(id);
    }

    void CleanBuffer() {
        buffer_.clear();
    }

  private:

    DataGear*   data_gear_;
    DataBuffer  buffer_;

    DataID NewDataID();

    void DeleteDataID(DataID id);

    VirtualData::Ptr LoadChunk(const std::string& chunk, lua_CFunction loader);

};

std::string NameConversion(const std::string& name);

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUAWRAPPER_H_ */
