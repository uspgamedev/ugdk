
%{
#include <functional>
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
%}

namespace std {
    template<class T>
    class function;
}

%{
template<class T>
struct remove_function;

template<class T>
struct remove_function< std::function<T> > {
    typedef T type;
};
%}

#ifdef SWIGLUA
%{
#include <ugdk/script/languages/lua/luadata.h>
#include <ugdk/script/languages/lua/luawrapper.h>
%}
#endif

#ifdef SWIGLUA
%typemap(in) std::function< T > {
    ugdk::script::lua::LuaWrapper* wrapper = static_cast<ugdk::script::lua::LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
    ugdk::script::lua::LuaData* luadata = wrapper->NewLuaData();
    ugdk::script::VirtualData::Ptr data_ptr(luadata);
    
    lua_pushvalue(L, $input);
    luadata->UnsafePopValue();
    $1 = ugdk::script::VirtualObj(data_ptr).AsFunction< remove_function< $1_basetype >::type >();
}
#endif

