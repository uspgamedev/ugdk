#include <ugdk/script.h>

#include <ugdk/config/config.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

void force_link_function_ugdk_internal(void) { 
    extern int UGDK_MODULES_HEARTBEAT; 
    UGDK_MODULES_HEARTBEAT = 1;
}

namespace ugdk {
namespace script {

void InitScripts() {
#ifdef UGDK_LUA_ENABLED
    SCRIPT_MANAGER()->Register(new ugdk::script::lua::LuaWrapper());
#endif
#ifdef UGDK_PYTHON_ENABLED
    SCRIPT_MANAGER()->Register(new ugdk::script::python::PythonWrapper());
#endif
}

} // namespace script
} // namespace ugdk
