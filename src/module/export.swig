
%{

#define SWIG_GENERATED
#include <ugdk/script/defs.h>
#undef SWIG_GENERATED
#include <ugdk/script/type.h>

%}

%include "ugdk/script/defs.h"
%include "ugdk/script/type.h"

%define export_class(T)
%typemap(in) T* tp {
    /// Code to register type T.
    /// First get the T's virtual type.
    ugdk::script::VirtualType& t = ugdk::script::TypeRegistry<$*1_type>::type();
    /// And then register it for the language Lua.
    t.RegisterFromLang(
        $1_descriptor,
        ugdk::script::LANG(Lua)
    );
    $1 = NULL;
}
%template(RegisterType_##T) ::RegisterType<T>;
%typemap(in) T* tp;
%template(GetNull_##T) ::GetNull<T>;
%enddef

%luacode {
for k,v in pairs(proto) do
  m = string.match(k, "RegisterType_(.+)")
  if m then
    proto[k](proto["GetNull_"..m]())
  end
end
}
