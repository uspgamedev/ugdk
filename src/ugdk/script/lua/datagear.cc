
#include <cstdio>

#include <ugdk/script/lua/datagear.h>
#include <ugdk/script/lua/auxlib.h>

#include <ugdk/script/swig/swigluarun.h>

/*static int ll_SafeGenerateID(lua_State* L) {
    return 0;
}*/

namespace ugdk {
namespace script {
namespace lua {

/// Public:

template <class T>
class Result {
  public:
    static T Get(DataGear& gear, lua_CFunction safe_unwrapper, DataID id,
                 T default_value) {
        gear->pushcfunction(safe_unwrapper);
        gear->pushudata(&gear);
        gear->pushinteger(id);
        T result = default_value;
        if (gear.TracedCall(2,1) == Constant::OK()) {
            result = gear->toprimitive<T>(-1);
            gear->pop(1);
        }
        return result;
    }
  private:
    Result() {}
};

const char* DataGear::UnwrapString_old (DataID id) {
    return Result<const char*>::Get(*this, UnwrapString, id, NULL);
}

bool DataGear::UnwrapBoolean(DataID id) {
    return Result<bool>::Get(*this, SafeUnwrapBoolean, id, false);
}

bool DataGear::GetData (DataID id) {
    if (!PushDataTable()) return false;
    // DataTable is at local index 1;
    L_.rawgeti(-1, id);     // [DT, DT.id]
    if (L_.isnil(-1)) {
        L_.pop(2);                         // []
        return false;
    }
    L_.remove(-2);                         // [DT.id]
    return true;
}

bool DataGear::SetData (DataID id) {
    // [data]
    if (!PushDataTable()) return false;
                        // [data,DT]
    L_.pushvalue(-2);   // [data,DT,data]
    L_.rawseti(-2, id); // [data,DT]
    L_.pop(2);          // []
    return true;
}

const Constant DataGear::DoFile (const char* filename) {
    {
        const Constant result = L_.aux().loadfile(filename);
        if (result != Constant::OK()) return Report(result);
    }   // [file]
    {
        L_.newtable();  // temp env table
        L_.newtable();  // temp env table's metatable
        L_.getfenv(-3);
        L_.setfield(-2, "__index"); // mttab.__index = _ENV
        L_.setmetatable(-2);        // setmetatable(temp, mttab)
        L_.setfenv(-2);             // setfenv(file,temp)
    }
    L_.pushvalue(-1); // make copy of file
    const Constant result = TracedCall(0, 0);
    if (result == Constant::OK()) {
        L_.pushnil();
        L_.setmetatable(-2);        // setmetatable(file,nil)
        L_.getfenv(-1);             // return getfenv(file)
    }
    if (L_.istable(-1))
        LuaMsg("Environtment table successfully retrieved.\n");
    else return Constant::err::ERR();
    return result;
}

/// Private:

int DataGear::GenerateID(lua_State* L) {
    State L_(L);

    L_.settop(1);
    GETARG(L_, 1, DataGear, dtgear);
    L_.settop(0);

    if (!dtgear.PushDataTable())
        L_.pushinteger(LUA_NOREF);
    else {
        L_.pushboolean(false);
        DataID generated = L_.aux().ref(-2);
        L_.settop(0);
        L_.pushinteger(generated);
    }

    return 1;
}

int DataGear::DestroyID(lua_State* L) {
    State L_(L);

    L_.settop(2);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    L_.settop(0);

    if (dtgear.PushDataTable()) {
        L_.aux().unref(-1, id);
        L_.pop(1);
    }

    return 0;
}

int DataGear::WrapData(lua_State* L) {
    State L_(L);

    L_.settop(4);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    UData data = L_.touserdata(3);
    GETARGPTR(L_, 4, swig_type_info, type);
    L_.settop(0);

    SWIG_NewPointerObj(L_, data, type, 0);
    if (!dtgear.SetData(id))
        return luaL_error(L, "Could not set data with id #%d", id);

    return 0;
}

int DataGear::UnwrapData(lua_State* L) {
    State L_(L);

    L_.settop(3);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    GETARGPTR(L_, 3, swig_type_info, type);
    L_.settop(0);

    void *data = NULL; // dummy

    if (dtgear.GetData(id) &&
        SWIG_IsOK(SWIG_ConvertPtr(L, -1, &data, type, 0))) {
        L_.pushudata(data);
    } else L_.pushnil();

    return 1;

}

int DataGear::UnwrapString(lua_State* L) {
    State L_(L);

    L_.settop(2);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    L_.settop(0);

    if (!dtgear.GetData(id)) {
        L_.pushnil();
    } // else the string will already be on top

    if (!L_.isstring(-1))
        return luaL_error(L, "Could not unwrap string from id #%d", id);

    return 1;

}

int DataGear::SafeUnwrapBoolean(lua_State* L) {
    State L_(L);

    L_.settop(2);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    L_.settop(0);

    if (!dtgear.GetData(id)) {
        L_.pushnil();
    } // else the value will already be on top

    return 1;

}

bool DataGear::PushDataTable() {
    L_.rawgeti(Constant::REGISTRYINDEX(), datatable_id_);
    if (!L_.istable(-1)) {
        L_.pop(1);
        return false;
    }
    return true;
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
