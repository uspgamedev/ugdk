
#ifndef UGDK_SCRIPT_LUA_HEADER_H_
#define UGDK_SCRIPT_LUA_HEADER_H_

#include <string>

extern "C" {

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#ifndef LUA_OK
# define LUA_OK (0)
#endif

}

namespace ugdk {
namespace script {
namespace lua {

typedef int     DataID;
typedef void*   UData;

template <class T>
UData AsUData(T* p) { return static_cast<UData>(p); }

template <class T>
UData AsUData(const T* p) {
    return AsUData(const_cast<T*>(p));
}

#define DECLARE_LUA_CONSTANT(category,name) \
  static const Constant name () { return Constant(LUA_##category##name); }

/// Represents a constant value from the Lua library.
class Constant {

  public:

    /// Constructs a Lua constant frm the result of a call to <i>c</i>.
    /**
     **  @param c - A callable object which must return <b>int</b> when called
     **             with zero arguments.
     */
    template <typename Callable>
    explicit Constant (Callable c) :
        value_(c()) {}

    int value() { return value_; }

    DECLARE_LUA_CONSTANT(,OK)
    DECLARE_LUA_CONSTANT(,REGISTRYINDEX)
    DECLARE_LUA_CONSTANT(,GLOBALSINDEX)

    struct err {
        DECLARE_LUA_CONSTANT(ERR,RUN)
        DECLARE_LUA_CONSTANT(ERR,MEM)
        DECLARE_LUA_CONSTANT(ERR,ERR)
        DECLARE_LUA_CONSTANT(ERR,SYNTAX)
        DECLARE_LUA_CONSTANT(ERR,FILE)
    };

    struct gc {
        DECLARE_LUA_CONSTANT(GC,STOP)
        DECLARE_LUA_CONSTANT(GC,RESTART)
        DECLARE_LUA_CONSTANT(GC,COLLECT)
        DECLARE_LUA_CONSTANT(GC,COUNT)
        DECLARE_LUA_CONSTANT(GC,COUNTB)
        DECLARE_LUA_CONSTANT(GC,STEP)
        DECLARE_LUA_CONSTANT(GC,SETPAUSE)
        DECLARE_LUA_CONSTANT(GC,SETSTEPMUL)
    };

    bool operator == (const Constant& st) const { return value_ == st.value_; }
    bool operator != (const Constant& st) const { return value_ != st.value_; }
    operator int() const { return value_; }

  private:

    Constant (int value) : value_(value) {}

    int value_;

};

#undef DECLARE_LUA_CONSTANT

inline void LuaMsg (const char *format, ...) {
    printf("[Lua] ");
    va_list list;
    va_start(list,format);
    vprintf(format, list);
    va_end(list);
}

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_LUA_HEADER_H_ */
