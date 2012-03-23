
%define enable_disown(T, name)
ifdef SWIGLUA
%typemap(in) T name {
    if (!SWIG_IsOK(SWIG_ConvertPtr(L,$input,(void**)&$1,$1_type,SWIG_POINTER_DISOWN))){
        SWIG_fail_ptr("$symname",$input,$1_type);
    }
}
#endif
%enddef

%define disable_disown(T, name)
%typemap(in) T name;
%enddef
