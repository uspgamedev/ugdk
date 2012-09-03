#include <ugdk/script.h>

#include <ugdk/config/config.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/languages/lua/luawrapper.h>
#include <ugdk/script/languages/python/pythonwrapper.h>

namespace ugdk {
namespace script {

void InitScripts() {
#ifdef UGDK_USING_LUA
    SCRIPT_MANAGER()->Register(new ugdk::script::lua::LuaWrapper());
#endif
#ifdef UGDK_USING_PYTHON
    SCRIPT_MANAGER()->Register(new ugdk::script::python::PythonWrapper());
#endif
}

} // namespace script
} // namespace ugdk
