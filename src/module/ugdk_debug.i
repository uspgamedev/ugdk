%module ugdk_debug

%include <module/util_ugdk.swig>

%{

#include <ugdk/debug/log.h>
#include <ugdk/debug/profiler.h>

%}

%import(module="ugdk_system") <ugdk/structure/types.h>

%include <ugdk/debug/log.h>
%include <ugdk/debug/profiler.h>
 
confirm_exports(ugdk_debug)
