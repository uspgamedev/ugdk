
#ifndef UGDK_MODULES_H_
#define UGDK_MODULES_H_

#include <ugdk/script.h>

namespace ugdk {

/// Loads all UGDK's Lua modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterLuaModules(script::lua::LuaWrapper* wrapper);

/// Loads all UGDK's Python modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterPythonModules(script::python::PythonWrapper* wrapper);

}

#endif /* UGDK_MODULES_H_ */
