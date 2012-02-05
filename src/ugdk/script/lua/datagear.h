
#ifndef UGDK_SCRIPT_LUA_DATAGEAR_H_
#define UGDK_SCRIPT_LUA_DATAGEAR_H_

#include <ugdk/script/type.h>
#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>
#include <ugdk/script/lua/basegear.h>

namespace ugdk {
namespace script {
namespace lua {

class DataGear : public BaseGear {

  public:

    DataGear(lua_State* L, DataID datatable_id) :
      BaseGear(L),
      datatable_id_(datatable_id) {}

    ~DataGear() {}

    int GenerateID();

    bool DestroyID(DataID id);

    // [-0,+1]
    bool GetData (DataID id);

    // [-1,+0]
    bool SetData (DataID id);

    // [-1,+0]
    void* UnwrapData (const VirtualType& type);

    void WrapData(void *data, const VirtualType& type);

    // [-0,+1]
    const Constant DoFile (const char* filename);

  private:

    DataID datatable_id_;

    /// [-0,+(0|1),-]
    bool PushDataTable();


};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_DATAGEAR_H_ */
