%module ugdk_time

%include <module/export.swig>

%{

#include <ugdk/time/timeaccumulator.h>
#include <ugdk/time/manager.h>
#include <ugdk/time/module.h>

%}

%import(module="ugdk_system") <ugdk/common/types.h>

%include <ugdk/time/timeaccumulator.h>
%include <ugdk/time/manager.h>
%include <ugdk/time/module.h>

namespace ugdk {
namespace time {
    export_class(TimeAccumulator)
    export_class(Manager)
}
}
 
confirm_exports(ugdk_time)
