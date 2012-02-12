
#ifndef UGDK_SCRIPT_LUA_DATAGEAR_H_
#define UGDK_SCRIPT_LUA_DATAGEAR_H_

#include <ugdk/script/type.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>
#include <ugdk/script/lua/basegear.h>
#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace script {
namespace lua {

class DataGear : public BaseGear, private ugdk::util::Uncopyable {

  public:

    DataGear(lua_State* L, DataID datatable_id) :
      BaseGear(L),
      datatable_id_(datatable_id) {}

    ~DataGear() {
        L_.aux().unref(Constant::REGISTRYINDEX(), datatable_id_);
        datatable_id_ = LUA_NOREF;
        L_.close();
    }

    DataID GenerateID();

    bool DestroyID(DataID id);

    // [-0,+0]
    bool WrapData(DataID id, void *data, const VirtualType& type);

    // [-0,+0]
    void* UnwrapData (DataID id, const VirtualType& type);

    // [-0,+0]
    const char* UnwrapString (DataID id);

    // [-0,+0]
    bool UnwrapBoolean (DataID id);

    // [-0,+1]
    bool GetData (DataID id);

    // [-1,+0]
    bool SetData (DataID id);

    // [-0,+1]
    const Constant DoFile (const char* filename);

  private:

    DataID datatable_id_;

    DataGear& operator=(const DataGear& rhs) {
        return *this;
    }

    /// Safely generates a data ID. [-1,+1,-]
    static int SafeGenerateID(lua_State* L);

    /// Safely destroys a data ID. [-2,+0,-]
    static int SafeDestroyID(lua_State* L);

    /// Safely wraps typed data to a data ID. [-4,+0,-]
    static int SafeWrapData(lua_State* L);

    /// Safely unwraps typed data from a data ID. [-3,+1,-]
    static int SafeUnwrapData(lua_State* L);

    /// Safely unwraps a string from a data ID. [-2,+1,-]
    static int SafeUnwrapString(lua_State* L);

    /// Safely unwraps a boolean from a data ID. [-2,+1,-]
    static int SafeUnwrapBoolean(lua_State* L);

    /// [-0,+(0|1),-]
    bool PushDataTable();


};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_DATAGEAR_H_ */
