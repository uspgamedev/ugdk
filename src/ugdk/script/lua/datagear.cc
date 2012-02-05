
#include <cstdio>

#include <ugdk/script/lua/datagear.h>
#include <ugdk/script/lua/auxlib.h>

#include <ugdk/script/swig/swigluarun.h>

namespace ugdk {
namespace script {
namespace lua {

/// Public:

int DataGear::GenerateID() {
    if (!PushDataTable()) return LUA_NOREF;
    DataID generated = L_.aux().ref(-1);
    L_.pop(1);
    return generated;
}

bool DataGear::DestroyID(DataID id) {
    if (!PushDataTable()) return false;
    L_.aux().unref(-1, id);
    L_.pop(1);
    return true;
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
    puts("\tSETP 6.1");
    // [data]
    if (!PushDataTable()) return false;
    puts("\tSETP 6.2");
                        // [data,DT]
    L_.pushvalue(-2);   // [data,DT,data]
    L_.rawseti(-2, id); // [data,DT]
    /*
    L_.push(id);        // [data,DT,id]
    puts("\tSETP 6.3");
    L_.pushvalue(-3);   // [data,DT,id,data]
    puts("\tSETP 6.4");
    L_.settable(-3);    // [data,DT]
    */
    puts("\tSETP 6.5");
    L_.pop(2);          // []
    puts("\tSETP 6.6");
    return true;
}

void* DataGear::UnwrapData (const VirtualType& type) {
    void* data = NULL;
    SWIG_ConvertPtr(L_, -1, &data, type.FromLang(LANG(Lua)), 0);
    L_.pop(1);
    return data;
}

void DataGear::WrapData(void *data, const VirtualType& type) {
    SWIG_NewPointerObj(L_, data, type.FromLang(LANG(Lua)), 0);
}

const Constant DataGear::DoFile (const char* filename) {
    {
        const Constant result = L_.aux().loadfile(filename);
        if (result != Constant::OK()) return Report(result);
    }   // [file]
    puts("SETP 0");
    {
        L_.newtable();  // temp env table
        L_.newtable();  // temp env table's metatable
        puts("SETP 0.1");
        L_.getfenv(-3);
        L_.setfield(-2, "__index"); // mttab.__index = _ENV
        L_.setmetatable(-2);        // setmetatable(temp, mttab)
        L_.setfenv(-2);             // setfenv(file,temp)
        puts("SETP 0.2");
    }
    puts("SETP 1");
    L_.pushvalue(-1); // make copy of file
    const Constant result = TracedCall(0, 0);
    if (result == Constant::OK()) {
        L_.pushnil();
        L_.setmetatable(-2);        // setmetatable(file,nil)
        L_.getfenv(-1);             // return getfenv(file)
    }
    if (L_.istable(-1))
        puts("Environtment table successfully retrieved.");
    else return Constant::err::ERR();
    puts("SETP 2");
    return result;
}

/// Private:

bool DataGear::PushDataTable() {
    L_.rawgeti(Constant::REGISTRYINDEX(), datatable_id_);
    /*
    L_.push(datatable_id_);
    L_.gettable(Constant::REGISTRYINDEX());
    */
    if (!L_.istable(-1)) {
        L_.pop(1);
        return false;
    }
    return true;
}
} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
