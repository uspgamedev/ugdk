%module ugdk_time

%include <module/export.swig>

%{

#include <ugdk/time/timeaccumulator.h>
#include <ugdk/time/timemanager.h>

%}

%import <module/base.i>

%include <ugdk/time/timeaccumulator.h>
%include <ugdk/time/timemanager.h>

namespace ugdk {
namespace time {
    export_class(TimeAccumulator)
    export_class(TimeManager)
}
}
 
confirm_exports(ugdk_time)
