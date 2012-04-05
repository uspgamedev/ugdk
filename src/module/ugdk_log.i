%module ugdk_log

%include <module/export.swig>

%{

#include <ugdk/log/log.h>

%}

%include <ugdk/log/log.h>
 
confirm_exports(ugdk_log)
