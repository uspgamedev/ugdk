
#ifndef UGDK_SCRIPT_LUA_BASEGEAR_H_
#define UGDK_SCRIPT_LUA_BASEGEAR_H_

#include <vector>
#include <tuple>

#include <ugdk/script/lua/header.h>
#include <ugdk/script/lua/state.h>

#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace script {
namespace lua {

class BaseGear {

  public:

    class InternalSafeCall {

      public:

        ~InternalSafeCall() { gear_->settop(old_top_); }

        template <class A1>
        InternalSafeCall& Arg(A1 a1) {
            gear_->pushprimitive<A1>(a1);
            ++arg_num_;
            return *this;
        }

        template <class R>
        R GetResult(const R default_value) {
            R result = default_value;
            if (gear_.TracedCall(arg_num_,1) == Constant::OK())
                result = gear_->toprimitive<R>(-1);
            arg_num_ = 0;
            return result;
        }

        bool NoResult() {
            return gear_.TracedCall(arg_num_,0) == Constant::OK();
        }

      private:

        friend class BaseGear;

        InternalSafeCall(BaseGear& gear, lua_CFunction func) :
            gear_(gear),
            old_top_(gear->gettop()),
            arg_num_(1) {
            gear_->pushcfunction(func);
            gear_->pushudata(&gear_);
        }

        BaseGear&   gear_;
        int         old_top_;
        size_t      arg_num_;

    };

    InternalSafeCall SafeCall(lua_CFunction func) {
        return InternalSafeCall(*this, func);
    }

    State* operator->() { return &L_; }

    /// Makes a traced call to a function.
    /** Behaves exactly like lua_pcall, with the usual traceback function from
     ** Lua.
     **
     ** [-(nargs+1),+(nres|0),e]
     */
    const Constant TracedCall (int nargs, int nres);

  protected:

    friend class InternalSafeCall;

    State L_;

    BaseGear(lua_State* L) :
        L_(L) {}

    ~BaseGear() {}

    State& L() { return L_; }

    const Constant Report (const Constant& c);
};

#define GETARGPTR(L,i,T,name) \
    T* name = static_cast<T*>((L).touserdata(i))

#define GETARG(L,i,T,name) \
    T& name = *(static_cast<T*>((L).touserdata(i)))

} /* namespace lua */
} /* namespace script */
} /* namespace ugdk */

#endif /* UGDK_SCRIPT_LUA_BASEGEAR_H_ */
