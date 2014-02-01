
%include <module/export.swig>
%include <module/proxy.swig>
%include <module/ownership.swig>
%include <module/widetypes.swig>
%include <module/function.swig>

// Manually including cmath in the beginning prevents errors on some compilers
%begin %{

#ifdef _WIN32
#ifndef MSVC
#include <cmath>
#endif // MSVC
#endif // _WIN32

%}

