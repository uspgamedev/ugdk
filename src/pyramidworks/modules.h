
#ifndef PYRAMIDWORKS_MODULES_H_
#define PYRAMIDWORKS_MODULES_H_

#include <ugdk/script.h>

namespace pyramidworks {

/// Loads all PYRAMIDWORKS's Lua modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterLuaModules(ugdk::script::lua::LuaWrapper* wrapper);

/// Loads all PYRAMIDWORKS's Python modules using the given wrapper.
/**
 ** @param wrapper - The wrapper used to register the modules.
 */
void RegisterPythonModules(ugdk::script::python::PythonWrapper* wrapper);

}

#endif /* PYRAMIDWORKS_MODULES_H_ */
