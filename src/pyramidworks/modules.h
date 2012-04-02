
#ifndef PYRAMIDWORKS_MODULES_H_
#define PYRAMIDWORKS_MODULES_H_

#include <ugdk/script.h>

namespace ugdk {

/// Loads all PYRAMIDWORKS's Lua modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterLuaModules(script::lua::LuaWrapper* wrapper);

/// Loads all PYRAMIDWORKS's Python modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterPythonModules(script::python::PythonWrapper* wrapper);

}

#endif /* PYRAMIDWORKS_MODULES_H_ */
