
#ifndef UGDK_SCRIPT_LUA_DEFS_H_
#define UGDK_SCRIPT_LUA_DEFS_H_

#ifndef lua_h
extern "C" {
struct lua_State;
typedef struct lua_State lua_State;
typedef int (*lua_CFunction)(lua_State*);
}
#endif

#include <list>
#include <map>

namespace ugdk {
namespace script {
namespace lua {

typedef int                     DataID;
typedef std::list<DataID>       DataBuffer;
typedef std::map<DataID,DataID> DataMap;
typedef void*                   UData;

template <class T>
UData AsUData(T* p) { return static_cast<UData>(p); }

template <class T>
UData AsUData(const T* p) {
    return AsUData(const_cast<T*>(p));
}

typedef lua_CFunction inittype;

#define LUA_INIT_FUNCTION_NAME(name) luaopen_##name
#define LUA_INIT_FUNCTION_SIGNATURE(name) int LUA_INIT_FUNCTION_NAME(name)(lua_State*)
#define LUA_MODULE_NAME(name) ugdk::script::lua::NameConversion(#name)
#define LUA_NAMESPACE ::ugdk::script::lua

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_LUA_DEFS_H_ */
