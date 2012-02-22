
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

int DataGear::Execute(lua_State* L) {
    State L_(L);

    L_.settop(4);
    GETARG(L_, 1, DataGear, dtgear);
    DataID func_id = L_.aux().checkintteger(2);
    GETARG(L_, 3, DataBuffer, buffer);
    DataID result_id = L_.aux().checkintteger(4);
    L_.settop(0);

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return 0;

    // Pushes the function.
    dtgear.PushData(1, func_id);
    // Pushes the arguments.
    for (DataBuffer::iterator it = buffer.begin(); it != buffer.end(); ++it)
        dtgear.PushData(1, *it);
    // Calls the function. The result will be on index 2, the new stack top.
    L_.call(static_cast<int>(buffer.size()), 1);

    // Pops the result into the data table.
    dtgear.PopData(1,result_id);

    return 0;
}

int DataGear::GetField(lua_State* L) {
    State L_(L);

    L_.settop(4);
    GETARG(L_, 1, DataGear, dtgear);
    DataID container_id = L_.aux().checkintteger(2);
    GETARG(L_, 3, DataBuffer, buffer);
    DataID value_id = L_.aux().checkintteger(4);
    L_.settop(0);

    if (!buffer.size())
        return luaL_error(
            L,
            "At operation __index: attempt to get field without a key."
        );

    DataID key_id = buffer.front();

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return 0;

    // Pushes the container at index 2.
    dtgear.PushData(1, container_id);
    // Pushes the key at index 3.
    dtgear.PushData(1, key_id);
    // Gets the field value in the container using the key. The key is poped and
    // the field value is pushed, becoming the new stack top at index 3.
    L_.gettable(2);

    // Pops the field into the data table.
    dtgear.PopData(1,value_id);

    return 0;
}

int DataGear::SetField(lua_State* L) {
    State L_(L);

    L_.settop(4);
    GETARG(L_, 1, DataGear, dtgear);
    DataID container_id = L_.aux().checkintteger(2);
    GETARG(L_, 3, DataBuffer, buffer);
    DataID finalvalue_id = L_.aux().checkintteger(4);
    L_.settop(0);

    if (buffer.size() < 2)
        return luaL_error(
            L,
            "At operation __newindex: attempt to set field without a key or "
            "value."
        );

    DataID key_id = buffer.front(),
           newvalue_id = *(++buffer.begin());

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return 0;

    // Pushes the container at index 2.
    dtgear.PushData(1, container_id);
    // Pushes the key at index 3.
    dtgear.PushData(1, key_id);
    // Pushes the new value at index 4.
    dtgear.PushData(1, newvalue_id);
    // Sets the field value in the container using the key and the new value.
    // Both are poped and nothing is pushed, so the new stack top is 2.
    L_.settable(2);

    // Now we get the field again to return it.
    // Pushes the key at index 3.
    dtgear.PushData(1, key_id);
    // Gets the field value in the container using the key. The key is poped and
    // the field value is pushed, becoming the new stack top at index 3.
    L_.gettable(2);

    // Pops the field into the data table.
    dtgear.PopData(1,finalvalue_id);

    return 0;
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
