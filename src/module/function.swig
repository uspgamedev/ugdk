
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

namespace {
ugdk::script::lua::LuaData* CreateLuaDataFrom(lua_State *L, int index) {
    ugdk::script::lua::LuaWrapper* wrapper = static_cast<ugdk::script::lua::LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
    ugdk::script::lua::LuaData* luadata = wrapper->NewLuaData();
    lua_pushvalue(L, index);
    luadata->UnsafePopValue();
    return luadata;
}
}
%}
#endif

#ifdef SWIGLUA
%typemap(in) std::function< T > {
    ugdk::script::lua::LuaData* luadata = CreateLuaDataFrom(L, $input);
    $1 = ugdk::script::VirtualObj( ugdk::script::VirtualData::Ptr(luadata) ).AsFunction< remove_function< $1_basetype >::type >();
}

%typemap(in) const std::function< T >& {
    ugdk::script::VirtualData::Ptr data(CreateLuaDataFrom(L, $input));
    $1 = new $1_basetype(ugdk::script::VirtualObj(data).AsFunction<remove_function< $1_basetype >::type>());
}

#endif

%typemap(freearg) const std::function< T >& {
    delete $1;
}

