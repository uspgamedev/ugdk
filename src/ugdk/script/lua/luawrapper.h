
#ifndef UGDK_SCRIPT_LUA_LUAWRAPPER_H_
#define UGDK_SCRIPT_LUA_LUAWRAPPER_H_

#include <string>
#include <list>
#include <vector>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/bootstrapgear.h>
#include <ugdk/script/lua/datagear.h>
#include <ugdk/script/langwrapper.h>

namespace ugdk {
namespace script {
namespace lua {

class LuaData;
typedef ugdk::script::InheritableLangWrapper<lua_CFunction> LuaWrapperBase;

class LuaWrapper: public LuaWrapperBase {

  public:

    LuaWrapper() :
        LuaWrapperBase("lua",LANG(Lua)),
        data_gear_(NULL) {}
    ~LuaWrapper() {
        if (data_gear_) Finalize();
    }

    /// Overwritten methods.

    bool Initialize();

    void Finalize();

    VirtualData::Ptr NewData();

    LuaData* NewLuaData();

    void ExecuteCode(const std::string& code) {
        LoadChunk(code, DataGear::DoString);
    }

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

    DataID NewDataID() {
        return data_gear()
            .SafeCall(DataGear::GenerateID)
            .GetResult<DataID>(LUA_NOREF);
    }

    void DeleteDataID(DataID id) {
        data_gear()
            .SafeCall(DataGear::DestroyID)
            .Arg(id)
            .NoResult();
    }

    VirtualData::Ptr LoadChunk(const std::string& chunk, lua_CFunction loader);

};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_LUAWRAPPER_H_ */
