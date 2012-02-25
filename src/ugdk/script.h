
#ifndef UGDK_SCRIPT_H_
#define UGDK_SCRIPT_H_

namespace ugdk {
namespace script {

// Public classes
class LangWrapper;
class VirtualData;
class VirtualObj;

// Managers
class ScriptManager;

namespace lua {

// Public Lua classes
class LuaData;

// Lua Managers
class LuaWrapper;

} /* namespace lua */

namespace python {

// Public Python classes
class PythonData;

// Python Managers
class PythonWrapper;

} /* namespace python */

}  // namespace script
}  // namespace ugdk

#endif /* UGDK_SCRIPT_H_ */
