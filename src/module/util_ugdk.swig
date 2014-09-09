
%include <module/export.swig>
%include <module/proxy.swig>
%include <module/ownership.swig>
%include <module/widetypes.swig>
%include <module/function.swig>
%include <module/unique_ptr.swig>

// Manually including cmath in the beginning prevents errors on some compilers
%begin %{

#ifdef _WIN32
#ifndef _MSC_VER
#include <cmath>
#endif // _MSC_VER
#endif // _WIN32

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4701 )
#endif

%}
