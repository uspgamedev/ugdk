
#include <cstdio>

#include <ugdk/script/lua/basegear.h>
#include <ugdk/script/lua/auxlib.h>

#include <ugdk/script/lua/native/traceback.h>

namespace ugdk {
namespace script {
namespace lua {

/// Protected:

const Constant BaseGear::Report (const Constant& c) {
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

const Constant BaseGear::TracedCall (int nargs, int nres) {
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
