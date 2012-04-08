
%{
#include <ugdk/script/virtualobj.h>
#include <ugdk/script/scriptmanager.h>
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
Mainly, they need to be named ClassProxy, and their construct receive a ugdk::script::VirtualObj&
*/


#ifdef SWIGLUA
%define proxy_class(T)
%typemap(in) T* {
    $1 = 0;
}

%typemap(out) T* {
    lua_pushnil( L);
}
%enddef
#elif SWIGPYTHON
%define proxy_class(T)
%typemap(in) T* {
    printf("starting typemap in\n");
    int resConvert = 0;
    void *_argpObj = 0 ;
    resConvert = SWIG_ConvertPtr($input, &_argpObj, $1_descriptor, 0 | 0 );
    printf("conversion result = %d\n", resConvert);
    if (!SWIG_IsOK(resConvert)) {
        printf("converting arg to proxy class\n");
        Py_INCREF($input);
        ugdk::script::python::PythonWrapper* wrapper = static_cast<ugdk::script::python::PythonWrapper*>( SCRIPT_MANAGER()->GetWrapper("Python") );
        ugdk::script::VirtualData::Ptr vdata( new ugdk::script::python::PythonData(wrapper, $input, true) ); 
        $1 = new T##Proxy( ugdk::script::VirtualObj(vdata) );
    }
    else {
        $1 = reinterpret_cast< T* >(_argpObj);
    }
}

%typemap(out) T* {
    printf("starting typemap out [%s](%d)\n", typeid($1).name(), $1);
    T##Proxy* __proxy_result = dynamic_cast< T##Proxy* > ($1);
    if (__proxy_result == NULL) {
        /*the object to convert is not of his Proxy class type... Use default SWIG conversion.*/
        $result = SWIG_NewPointerObj(SWIG_as_voidptr($1), $1_descriptor,  0 | 0 );
    }
    else {
        printf("converting object back to proxy instance in script.\n");
        $result = static_cast<PyObject*>(__proxy_result->get_proxy_vobj().unsafe_data() );
    }
}
%enddef
#endif


