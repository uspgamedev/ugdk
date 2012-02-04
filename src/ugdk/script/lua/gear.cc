
#include <ugdk/script/lua/gear.h>
#include <ugdk/script/lua/auxlib.h>
#include <ugdk/script/lua/native.h>

#include <ugdk/script/swig/swigluarun.h>

namespace ugdk {
namespace script {
namespace lua {

using std::vector;

/// Public:

void Gear::LoadLibs () {
    // luaL_checkversion(L.get());  // TODO-lua5.2: add this.
    L_.gc(Constant::gc::STOP(), 0);
    L_.aux().openlibs();
    L_.gc(Constant::gc::RESTART(), 0);
}

void Gear::PreloadModule(const vector<Module>& modules) {
    L_.getglobal(LUA_LOADLIBNAME);             // [pack]
    L_.getfield(-1, "preload");                // [pack,preload]
    vector<Module>::const_iterator it = modules.begin();
    for (; it != modules.end(); ++it) {
        L_.push(it->init_func_);      // [pack,preload,init_func]
        L_.setfield(-2, it->name_.c_str());    // [pack,preload]
    }
    L_.pop(2);                                 // []
}

void Gear::CreateDatatable() {
    L_.push(datatable_id_);
    L_.newtable();
    L_.settable(Constant::REGISTRYINDEX());
}

bool Gear::GetData (DataID id) {
    if (!PushDataTable()) return false;
    // DataTable is at local index 1;
    L_.push(id);   // [DT,id]
    L_.gettable(-2);                       // [DT,DT.id]
    if (L_.isnil(-1)) {
        L_.pop(2);                         // []
        return false;
    }
    L_.remove(-2);                         // [DT.id]
    return true;
}

bool Gear::SetData (DataID id) {
    // [data]
    if (!PushDataTable()) return false;
                        // [data,DT]
    L_.push(id);        // [data,DT,id]
    L_.pushvalue(-3);   // [data,DT,id,data]
    L_.settable(-2);    // [data,DT]
    L_.pop(2);          // []
    return true;
}

void* Gear::UnwrapData (const VirtualType& type) {
    void* data = NULL;
    SWIG_ConvertPtr(L_, -1, &data, type.FromLang(LANG(Lua)), 0);
    L_.pop(1);
    return data;
}

void Gear::WrapData(void *data, const VirtualType& type) {
    SWIG_NewPointerObj(L_, data, type.FromLang(LANG(Lua)), 0);
}

const Constant Gear::DoFile (const char* filename) {
    {
        const Constant result = L_.aux().loadfile(filename);
        if (result != Constant::OK()) return Report(result);
    }   // [file]
    {
        L_.newtable();  // temp env table
        L_.newtable();  // temp env table's metatable
        L_.pushvalue(LUA_ENVIRONINDEX);
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
    return result;
}

const Constant Gear::LoadModule (const char* name, lua_CFunction loader) {
  L_.push(require);
  L_.push(name);
  L_.push(loader);
  const Constant result = TracedCall(2,1);
  if (result != Constant::OK())
      L_.pop(1);
  return result;
}

/// Private:

bool Gear::PushDataTable() {
    L_.push(datatable_id_);
    L_.gettable(Constant::REGISTRYINDEX());
    if (L_.isnil(-1)) {
        L_.pop(1);
        return false;
    }
    return true;
}

const Constant Gear::Report (const Constant& c) {
  if (c != Constant::OK() && !L_.isnil(-1)) {
    const char *msg = L_.tostring(-1);
    if (msg == NULL) msg = "(error object is not a string)";
    State::errormsg(msg);
    L_.pop(1);
    /* force a complete garbage collection in case of errors */
    L_.gc(Constant::gc::COLLECT(), 0);
  }
  return c;
}

const Constant Gear::TracedCall (int nargs, int nres) {
  int base = L_.gettop() - nargs;
  L_.push(traceback);
  L_.insert(base);
  const Constant result = L_.pcall(nargs, nres, base);
  L_.remove(base);
  return Report(result);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
