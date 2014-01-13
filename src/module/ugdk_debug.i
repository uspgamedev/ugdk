%module ugdk_debug

%include <module/export.swig>

%{

#include <ugdk/debug/log.h>
#include <ugdk/debug/profiler.h>

%}

%import(module="ugdk_system") <ugdk/structure/types.h>

%include <ugdk/debug/log.h>
%include <ugdk/debug/profiler.h>
 
confirm_exports(ugdk_debug)
