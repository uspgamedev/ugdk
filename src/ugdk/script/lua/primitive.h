
#ifndef UGDK_SCRIPT_LUA_PRIMITIVE_H_
#define UGDK_SCRIPT_LUA_PRIMITIVE_H_

#include <ugdk/script/lua/header.h>

namespace ugdk {
namespace script {
namespace lua {

#define CHECK_LUA_TYPE(name, type) (defined name) && (name == type)

/// Generates a Lua operation name.
#define LUA_OPNAME(name) lua_##name

/// Defines a Lua primitive operation.
/**
 ** Note that just defining does not make much. One has to define
 ** <i>operation cases</i> for it to have any meaning.
 **
 ** @param name - used to generate the operation name.
 ** @see DEFINE_LUA_PRIMITIVE_OPCASE
 */
#define DEFINE_LUA_PRIMITIVE_OP(name) \
    template <class T> \
    class LUA_OPNAME(name) { \
      private: \
        LUA_OPNAME(name)() {} \
        void primitive(); \
    }

#define DEFINE_LUA_PRIMITIVE_OPCASE(name, partial, type, ret, arg, exp) \
    template <partial> \
    class LUA_OPNAME(name)<type> { \
      public: \
        static ret primitive(lua_State* L, arg) { \
            exp; \
        } \
      private: \
        LUA_OPNAME(name)() {} \
    }

/** @name Lua primitive <b>push</b>.
 ** @{
 */

DEFINE_LUA_PRIMITIVE_OP(push);

#define DEFINE_LUA_PARTIAL_PUSH(partial, type, call) \
    DEFINE_LUA_PRIMITIVE_OPCASE(push, partial, type, void, type val, call)

#define DEFINE_LUA_FULL_PUSH(type, call) \
    DEFINE_LUA_PARTIAL_PUSH(, type, call)

#define DEFINE_LUA_SIMPLE_PUSH(type, name) \
    DEFINE_LUA_FULL_PUSH(type, lua_push##name(L, val))


DEFINE_LUA_PARTIAL_PUSH(class T, T*, lua_pushlightuserdata(L, AsUData<T>(val)));
DEFINE_LUA_FULL_PUSH(lua_CFunction, lua_pushcclosure(L, val, 0));
DEFINE_LUA_SIMPLE_PUSH(const char*, string);
DEFINE_LUA_SIMPLE_PUSH(bool, boolean);
DEFINE_LUA_SIMPLE_PUSH(int, integer);
DEFINE_LUA_SIMPLE_PUSH(double, number);
DEFINE_LUA_SIMPLE_PUSH(UData, lightuserdata);

#undef DEFINE_LUA_PARTIAL_PUSH
#undef DEFINE_LUA_FULL_PUSH
#undef DEFINE_LUA_SIMPLE_PUSH

/** @}
 */

/** @name Lua primitive <b>to</b>.
 ** @{
 */

DEFINE_LUA_PRIMITIVE_OP(to);

#define DEFINE_LUA_TO(type, call) \
    DEFINE_LUA_PRIMITIVE_OPCASE(to, , type, type, int index, return (call))

#define DEFINE_LUA_SIMPLE_TO(type, name) \
    DEFINE_LUA_TO(type, lua_to##name(L, index))

DEFINE_LUA_TO(const char*, lua_tolstring(L, index, NULL));
DEFINE_LUA_SIMPLE_TO(bool, boolean);
DEFINE_LUA_SIMPLE_TO(int, integer);
DEFINE_LUA_SIMPLE_TO(double, number);
DEFINE_LUA_SIMPLE_TO(UData, userdata);

#undef DEFINE_LUA_TO
#undef DEFINE_LUA_SIMPLE_TO

/** @}
 */

/** @name Lua primitive <b>is</b>.
 ** @{
 */

DEFINE_LUA_PRIMITIVE_OP(is);

#define DEFINE_LUA_IS(type, check) \
    DEFINE_LUA_PRIMITIVE_OPCASE(is, , type, bool, int index, \
                                return static_cast<bool>(check))

#define DEFINE_LUA_SIMPLE_IS(type, name) \
    DEFINE_LUA_IS(type, lua_is##name(L, index))

DEFINE_LUA_SIMPLE_IS(const char*, string);
DEFINE_LUA_IS(bool, !lua_isnone(L, index));
DEFINE_LUA_SIMPLE_IS(int, number);
DEFINE_LUA_SIMPLE_IS(double, number);

#undef DEFINE_LUA_IS
#undef DEFINE_LUA_SIMPLE_IS

/** @}
 */

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_PRIMITIVE_H_ */
