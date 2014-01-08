%module ugdk_debug

%include <module/export.swig>

%{

#include <ugdk/debug/log.h>

%}

%include <ugdk/debug/log.h>
 
confirm_exports(ugdk_debug)
