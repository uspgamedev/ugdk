
#include <cstdio>
#include <string>

#include <ugdk/script/languages/lua/datagear.h>
#include <ugdk/script/languages/lua/auxlib.h>

#include <ugdk/script/languages/lua/swigluarun.h>

namespace ugdk {
namespace script {
namespace lua {

/// Public:

static void dumpstack (lua_State* L) {
    LuaMsg("Dumping stack:\n");
    for (int i = 1; i <= lua_gettop(L); i++) {
        lua_getfield(L, LUA_GLOBALSINDEX, "print");
        lua_pushvalue(L, i);
        lua_call(L, 1, 0);
    }
}

int DataGear::GenerateID(lua_State* L) {
    State L_(L);

    L_.settop(1);
    GETARG(L_, 1, DataGear, dtgear);
    L_.settop(0);

    if (!dtgear.PushDataTable())
        L_.pushinteger(LUA_NOREF);
    else {
        //DataID generated = L_.aux().ref(-2);
        DataID generated = dtgear.idgen_.GenerateID();
        if (generated != 0) {
            L_.pushinteger(generated);
            L_.pushboolean(false);
            L_.settable(-3);
        } else generated = LUA_NOREF;
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
        //L_.aux().unref(-1, id);
        //L_.pop(1);
        DataID destroyed = dtgear.idgen_.ReleaseID(id);
        if (destroyed != id)
            LuaMsg("WARNING: Attempt to release Invalid lua data id.\n");
        else {
            L_.pushinteger(id);
            L_.pushnil();
            L_.settable(-3);
            L_.settop(0);
        }
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

    L_.settop(4);
    GETARG(L_, 1, DataGear, dtgear);
    DataID id = L_.aux().checkintteger(2);
    GETARGPTR(L_, 3, swig_type_info, type);
    bool disown = (L_.aux().checkintteger(4) != 0);
    L_.settop(0);

    void *data = NULL; // dummy

    if (dtgear.GetData(id) &&
        SWIG_IsOK(SWIG_ConvertPtr(L, -1, &data, type, disown ? SWIG_POINTER_DISOWN : 0))) {
        L_.pushudata(data);
    } else L_.pushnil();

    return 1;

}

static DataID MakeID(DataGear& dtgear) {
    dtgear->pushcfunction(DataGear::GenerateID);
    dtgear->pushudata(&dtgear);
    dtgear->call(1,1);
    DataID id = static_cast<DataID>(dtgear->tointeger(-1));
    dtgear->pop(1);
    return id;
}

int DataGear::UnwrapList(lua_State* L) {
    State L_(L);

    L_.settop(3);
    GETARG(L_, 1, DataGear, dtgear);
    DataID list_id = L_.aux().checkintteger(2);
    GETARG(L_, 3, DataBuffer, data_list);
    L_.settop(0);

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return luaL_error(L, "Data table unavailable.");

    // Pushes the unwrapping table. It will be on index 2.
    dtgear.PushData(1, list_id);

    if (!L_.istable(2))
        return luaL_error(L, "Could not unwrap table from id #%d", list_id);

    int i = 1;
    while (1) {
        // Pushes the i-th element. It will be on index 3.
        L_.rawgeti(2, i);
        if (L_.isnil(3)) break;
        DataID id = MakeID(dtgear);
        // Pops the i-th element, storing it into the datatable.
        dtgear.PopData(1, id);
        data_list.push_back(id);
        ++i;
    }

    return 0;
}

int DataGear::UnwrapTable(lua_State* L) {
    State L_(L);

    L_.settop(3);
    GETARG(L_, 1, DataGear, dtgear);
    DataID table_id = L_.aux().checkintteger(2);
    GETARG(L_, 3, DataMap, data_list);
    L_.settop(0);

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return luaL_error(L, "Data table unavailable.");

    // Pushes the unwrapping table. It will be on index 2.
    dtgear.PushData(1, table_id);

    if (!L_.istable(2))
        return luaL_error(L, "Could not unwrap table from id #%d", table_id);

    L_.pushnil(); // at index 3, but is soon removed.
    while (lua_next(L, 2)) {
        // Here we have the key at index 3 and the value at index 4.
        // Make indexes for both of them.
        DataID  key_id = MakeID(dtgear),
                value_id = MakeID(dtgear);
        // Pops the current value, storing it into the datatable.
        dtgear.PopData(1, value_id);
        // Copies the key. This leaves the key at the end, allowinf lua_next to
        // continue.
        L_.pushvalue(3);
        // Pops the current key's value, storing it into the datatable.
        dtgear.PopData(1, key_id);
        data_list[key_id] = value_id;
    }

    return 0;
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
    if (!L_.isfunction(-1) && !L_.istable(-1)) {
        return luaL_error(
            L,
            "WAT %d (%d) : %d",
            func_id, buffer.size(), result_id
        );
    }
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
    if (L_.isnil(2))
        return luaL_error(L, "Attempt to index a nil object.");
    // Pushes the key at index 3.
    dtgear.PushData(1, key_id);
    // Gets the field value in the container using the key. The key is poped and
    // the field value is pushed, becoming the new stack top at index 3.
    L_.gettable(2);

    // Pops the field into the data table.
    dtgear.PopData(1, value_id);

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

int DataGear::DoFile(lua_State* L) {
    State L_(L);

    L_.settop(3);
    GETARG(L_, 1, DataGear, dtgear);
    const char* filename = L_.aux().checkstring(2);
    DataID result_id = L_.aux().checkintteger(3);
    L_.settop(0);

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return luaL_error(
            L,
            "At operation dofile: could not acquire data table."
        );

    const Constant result = L_.aux().loadfile(filename);
    if (result != Constant::OK()) {
        dtgear.Report(result);
        return 0;
    }

#ifdef DEBUG
    std::string success_message = "Loaded module: '";
    success_message += filename;
    success_message += "'.\n";
    LuaMsg(success_message.c_str());
#endif

    L_.newtable();  // temp env table
    L_.newtable();  // temp env table's metatable
    L_.getfenv(-3);
    L_.setfield(-2, "__index"); // mttab.__index = _ENV
    L_.setmetatable(-2);        // setmetatable(temp, mttab)
    L_.setfenv(-2);             // setfenv(file,temp)

    L_.pushvalue(-1); // make copy of file
    L_.call(0, 0);
    L_.getfenv(-1);   // getfenv(file)

    dtgear.PopData(1, result_id);

    return 0;
}

int DataGear::DoString(lua_State* L) {
    State L_(L);

    L_.settop(3);
    GETARG(L_, 1, DataGear, dtgear);
    const char* chunk = L_.aux().checkstring(2);
    L_.settop(0);

    // Pushes the data table. It will be on index 1.
    if (!dtgear.PushDataTable())
        return luaL_error(
            L,
            "At operation dostring: could not acquire data table."
        );

    const Constant result = L_.aux().loadstring(chunk);
    if (result != Constant::OK()) {
        dtgear.Report(result);
        return 0;
    }

    L_.call(0, 0);

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

bool DataGear::HasValue (DataID id) {
    if (!PushDataTable()) return false;
    // DataTable is at local index 1;
    L_.rawgeti(-1, id);     // [DT, DT.id]
    bool has_value = !L_.isnil(-1);
    L_.pop(2);              // []
    return has_value;
}

/// Private:

bool DataGear::PushDataTable() {
    L_.getfield(Constant::REGISTRYINDEX(), "UGDK_LUA_DATATABLE");
    if (!L_.istable(-1)) {
        L_.pop(1);
        return false;
    }
    return true;
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
