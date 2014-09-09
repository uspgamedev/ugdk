
%{
#include <memory>
%}

namespace std {
    template<class T>
    class unique_ptr;
}

%{
template<class T>
struct remove_unique_ptr;

template<class T>
struct remove_unique_ptr< std::unique_ptr<T> > {
    typedef T type;
};
%}

#ifdef SWIGLUA
%typemap(in) std::unique_ptr< T >&& {
	using T = remove_unique_ptr< $1_basetype >::type;
	auto type_info = ::ugdk::script::TypeRegistry<T>::type().FromLang(::ugdk::script::LuaLang);
	T* ptr;
	if (!SWIG_IsOK(SWIG_ConvertPtr(L, $input, (void**) &ptr, type_info, SWIG_POINTER_DISOWN))) {
		SWIG_fail_ptr("$symname",$input,type_info);
	}
	$1 = new $1_basetype (ptr);
}

%typemap(freearg) std::unique_ptr< T >&& {
    delete $1;
}

#endif

