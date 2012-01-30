
#include <ugdk/script/lua/gear.h>
#include <ugdk/script/lua/auxlib.h>
#include <ugdk/script/lua/native.h>

namespace ugdk {
namespace script {
namespace lua {

void Gear::LoadLibs () {
    // luaL_checkversion(L.get());  // TODO-lua5.2: add this.
    lua_.gc(Constant::gc::STOP(), 0);
    lua_.aux().openlibs();
    lua_.gc(Constant::gc::RESTART(), 0);
}

const Constant Gear::DoFile (const char* filename) {
    const Constant result = lua_.aux().loadfile(filename);
    if (result != Constant::OK()) return Report(result);
    return TracedCall(0, LUA_MULTRET);
}

const Constant Gear::LoadModule (const char* name, lua_CFunction loader) {
  lua_.push(require);
  lua_.push(name);
  lua_.push(loader);
  const Constant result = TracedCall(2,1);
  if (result != Constant::OK())
      lua_.pop(1);
  return result;
}

const Constant Gear::Report (const Constant& c) {
  if (c != Constant::OK() && !lua_.isnil(-1)) {
    const char *msg = lua_.tostring(-1);
    if (msg == NULL) msg = "(error object is not a string)";
    State::errormsg(msg);
    lua_.pop(1);
    /* force a complete garbage collection in case of errors */
    lua_.gc(Constant::gc::COLLECT(), 0);
  }
  return c;
}

const Constant Gear::TracedCall (int nargs, int nres) {
  int base = lua_.gettop() - nargs;
  lua_.push(traceback);
  lua_.insert(base);
  const Constant result = lua_.pcall(nargs, nres, base);
  lua_.remove(base);
  return Report(result);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */
