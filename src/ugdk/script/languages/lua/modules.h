#ifndef UGDK_SCRIPT_LUA_MODULES_H_
#define UGDK_SCRIPT_LUA_MODULES_H_

#include <ugdk/script.h>
#include <ugdk/script/languages/lua/defs.h>

namespace ugdk {
namespace script {
namespace lua {

void AddModule(const Module<LUA_inittype> &);

void RegisterModules(LuaWrapper* wrapper);

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_LUA_MODULES_H_ */
