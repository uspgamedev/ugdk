
%{
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/baseproxy.h>
%}

#ifdef SWIGPYTHON
%{
#include <ugdk/script/languages/python/pythondata.h>
#include <ugdk/script/languages/python/pythonwrapper.h>
%}
#elif SWIGLUA
%{
#include <ugdk/script/languages/lua/luadata.h>
#include <ugdk/script/languages/lua/luawrapper.h>
%}
#endif

/*
This module defines a 'proxy_class(T)' macro for each target language.

proxy_class(T) creates the typemaps needed to seamless incorporation of the Proxy
classes in the script system. As such, it needs to be called before the %include's
in a module (it needs to go before the swig export definition (which is done via the %include)).

Use as following:    proxy_class(T)
where T is the original type (Ex.:  namespace1::subnamespace::Class)

Proxy classes need to follow a simple interface/ way to create them, so check your examples.
Mainly, they need to be named ClassProxy, and their constructor receives a ugdk::script::VirtualObj&
*/


#ifdef SWIGLUA
%{
#include <module/luaproxy.h>
%}

%luacode {
  local function hackmetatable (meta)
    local hacked = {}
    for k,v in pairs(meta) do
      if k == "__index" then
        hacked[k] = UGDK_proxymethods.getter
      elseif k == "__newindex" then
        hacked[k] = UGDK_proxymethods.setter
      else
        hacked[k] = v
      end
    end
    return hacked
  end
  function proxy (name)
    print ("Making proxy for "..name)
    local obj = {}
    setmetatable(obj, hackmetatable(swig[name]))
    return obj
  end
}

%init {
    ugdk::script::lua::ExportMetamethods(L);
}

%define proxy_class(T)
%typemap(in) T* {
    if (!SWIG_IsOK(SWIG_ConvertPtr(L, $input, (void**)&$1, $1_descriptor,0))) {
        ugdk::script::lua::LuaWrapper* wrapper = static_cast<ugdk::script::lua::LuaWrapper*>(SCRIPT_MANAGER()->GetWrapper("Lua"));
        ugdk::script::lua::LuaData* luadata = NULL;
        lua_pushvalue(L, $input);
        lua_getfield(L, -1, "__unsafedata");
        if (lua_isuserdata(L, -1)) {
            luadata = static_cast<ugdk::script::lua::LuaData*> (lua_touserdata(L, -1));
            lua_pop(L, 2);
            printf("Using proxy cache of type "#T" (%p)\n", luadata);
        } else {
            lua_pop(L, 1);
            luadata = wrapper->NewLuaData();
            lua_pushlightuserdata(L, static_cast<void*>(luadata));
            lua_setfield(L, -2, "__unsafedata");
            luadata->UnsafePopValue();
            printf("Created proxy cache of type "#T" (%p)\n", luadata);
        }
        //ugdk::script::VirtualData::Ptr vdata(luadata);
        //ugdk::script::VirtualObj vobj(vdata);
        $1 = T##Proxy::Get(static_cast<ugdk::script::VirtualData*>(luadata));
    }
}

%typemap(out) T* {
    using ugdk::script::lua::LuaData;
    T##Proxy* _proxy_result = dynamic_cast<T##Proxy*>($1);
    LuaData* luadata = NULL;
    if (_proxy_result == NULL) {
        SWIG_NewPointerObj(L, $result, $1_descriptor,0); SWIG_arg++;
    } else {
        luadata = static_cast<LuaData*>(_proxy_result->get_proxy_vobj().unsafe_data());
        luadata->UnsafePushValue();
        SWIG_arg++;
    }
}
%enddef
#elif SWIGPYTHON
%define proxy_class(T)
%typemap(in) T* {
    //printf("starting typemap in\n");
    int resConvert = 0;
    void *_argpObj = 0 ;
    resConvert = SWIG_ConvertPtr($input, &_argpObj, $1_descriptor, SWIG_POINTER_DISOWN );
    //printf("conversion result = %d\n", resConvert);
    if (!SWIG_IsOK(resConvert)) {
        Py_INCREF($input);
        ugdk::script::python::PythonWrapper* wrapper = static_cast<ugdk::script::python::PythonWrapper*>( SCRIPT_MANAGER()->GetWrapper("Python") );
        ugdk::script::VirtualData::Ptr vdata( new ugdk::script::python::PythonData(wrapper, $input, true) ); 
        ugdk::script::VirtualObj vobj(vdata);

        $1 = T##Proxy::Get( vobj );
        //printf("converting arg to proxy class [T##Proxy*=%d][PyObj*=%d][PyRefCoutn=%d]\n", (int)$1, (int)$input, (int)$input->ob_refcnt);
    }
    else {
        $1 = reinterpret_cast< T* >(_argpObj);
    }
}

%typemap(out) T* {
    //printf("starting typemap out [TypeId=%s](T##Proxy*=%d)\n", typeid($1).name(), (int)$1);
    T##Proxy* __proxy_result = dynamic_cast< T##Proxy* > ($1);
    if (__proxy_result == NULL) {
        /*the object to convert is not of his Proxy class type... Use default SWIG conversion.*/
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $1_descriptor,  SWIG_POINTER_DISOWN );
    }
    else {
        $result = static_cast<PyObject*>(__proxy_result->get_proxy_vobj().unsafe_data() );
        Py_INCREF($result);
        //printf("converting object back to proxy instance in script. [PyObj*=%d][PyRefCount=%d]\n", (int)$result, (int)$result->ob_refcnt);
    }
}
%enddef
#endif


/*************************************************************/
#ifdef SWIGLUA
%define void_class()
%typemap(in) void* {
    $1 = (void*)lua_touserdata(L, $input);
}

%typemap(out) void* {
    lua_pushlightuserdata(L, $result); SWIG_arg++;
}
%enddef
#elif SWIGPYTHON
%define void_class()
%typemap(in) void* {
    //printf("starting void in [PyObj*=%d][PyRefCount=%d]\n", (int)$input, (int)$input->ob_refcnt);
    //Py_INCREF($input);
    $1 = static_cast<void*>($input);
}

%typemap(out) void* {
    $result = static_cast<PyObject*>( $1 );
    //printf("starting void out (void*=%d) [PyRefCount=%d]\n", (int)$1, (int)$result->ob_refcnt);
    Py_INCREF($result);
}
%enddef
#endif
