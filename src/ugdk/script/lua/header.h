
#ifndef UGDK_SCRIPT_LUA_HEADER_H_
#define UGDK_SCRIPT_LUA_HEADER_H_

#include <ugdk/portable/tr1.h>

#include FROM_TR1(memory)

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

typedef std::tr1::shared_ptr<lua_State> StatePtr;

typedef int DataID;

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

struct Module {
    Module(const std::string& name, lua_CFunction init_func) :
        name_(name),
        init_func_(init_func) {}
    std::string     name_;
    lua_CFunction   init_func_;
};

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */


#endif /* UGDK_SCRIPT_LUA_HEADER_H_ */
