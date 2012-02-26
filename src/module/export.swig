
%{

#define SWIG_GENERATED
#include <ugdk/script/defs.h>
#undef SWIG_GENERATED
#include <ugdk/script/type.h>

%}

%include "ugdk/script/defs.h"
%include "ugdk/script/type.h"

#ifdef SWIGPYTHON
	#define CURRENT_LANG ugdk::script::LANG(Python)
#elif SWIGLUA
	#define CURRENT_LANG ugdk::script::LANG(Lua)
/* Else not used on purpose, so that some shit happens when someone tries to use
   this for any non-supported language.*/
#endif

%define export_class(T)
%typemap(in) T* tp {
    /// Code to register type T.
    /// First get the T's virtual type.
    ugdk::script::VirtualType& t = ugdk::script::TypeRegistry<$*1_type>::type();
    /// And then register it for the language Lua.
    t.RegisterFromLang(
        $1_descriptor,
        CURRENT_LANG
    );
    $1 = NULL;
}
%template(RegisterType_##T) ::RegisterType<T>;
%typemap(in) T* tp;
%template(GetNull_##T) ::GetNull<T>;
%enddef

/*confirm_exports should use the target language to somehow call the
  RegisterType_* functions in the module passing the return value
  from the GetNull_* functions as argument.*/
%define confirm_exports(modname)
#ifdef SWIGLUA
%luacode {
print "Hey there!"
for k,v in pairs(modname) do
  m = string.match(k, "RegisterType_(.+)")
  print(k,v)
  if m then
    print "Something may be registered."
    v(modname["GetNull_"..m]())
  end
end
}
#elif SWIGPYTHON
%pythoncode {
print "Python modname confirm exports!"
module = __import__(__name__)
for key,value in module.__dict__.items():
	if key.startswith("RegisterType_"):
		arg = module.__dict__["GetNull_"+key[13:] ]()
		value(arg)
}
#endif
%enddef
