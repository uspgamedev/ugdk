
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

    /// Generates a data ID. [-1,+1,-]
    static int GenerateID(lua_State* L);

    /// Safely destroys a data ID. [-2,+0,-]
    static int DestroyID(lua_State* L);

    /// Safely wraps typed data to a data ID. [-4,+0,-]
    static int WrapData(lua_State* L);

    /// Safely wraps primitive data to a data ID. [-3,+0,-]
    template <class T>
    static int WrapPrimitive(lua_State* L);

    /// Safely unwraps typed data from a data ID. [-3,+1,-]
    static int UnwrapData(lua_State* L);

    /// Safely unwraps a priitive value from a data ID. [-2,+1,-]
    template <class T>
    static int UnwrapPrimitive(lua_State* L);

    /// Safely unwraps a string from a data ID. [-2,+1,-]
    static int UnwrapString(lua_State* L);

    // [-0,+0]
    const char* UnwrapString_old (DataID id);

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

    /// Safely unwraps a boolean from a data ID. [-2,+1,-]
    static int SafeUnwrapBoolean(lua_State* L);

    /// [-0,+(0|1),-]
    bool PushDataTable();


};

template <class T>
int DataGear::UnwrapPrimitive(lua_State* L) {
    State L_(L);

    L_.settop(2);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    L_.settop(0);

    if (!dtgear.GetData(id)) {
        L_.pushnil();
    } // else the string will already be on top

    if (!L_.isprimitive<T>(-1))
        return luaL_error(L, "Could not unwrap primitive from id #%d", id);

    return 1;
}

template <class T>
int DataGear::WrapPrimitive(lua_State* L) {
    State L_(L);

    L_.settop(3);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    //T value = L_.aux().checkprimitive<T>(3);
    //UData data = L_.touserdata(3);
    //GETARGPTR(L_, 4, swig_type_info, type);
    //L_.settop(0);

    if (!L_.isprimitive<T>(3) || !dtgear.SetData(id))
        return luaL_error(L, "Could not wrap primitive with id #%d", id);

    //SWIG_NewPointerObj(L_, data, type, 0);
    //if (!dtgear.SetData(id))
    //    return luaL_error(L, "Could not wrap primitive with id #%d", id);

    return 0;
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_DATAGEAR_H_ */
