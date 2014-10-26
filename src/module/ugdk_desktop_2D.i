%module ugdk_desktop

%include <module/util_ugdk.swig>

%{

#include <ugdk/desktop/window.h>
#include <ugdk/desktop/windowsettings.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/desktop/module.h>

%}

%import(module="ugdk_system") <ugdk/structure/types.h>
%import(module="ugdk_math") <ugdk/math.h>

%include <ugdk/desktop/window.h>
%include <ugdk/desktop/windowsettings.h>
%include <ugdk/desktop/manager.h>
%include <ugdk/desktop/module.h>

namespace ugdk {
namespace desktop {
    export_class(Window)
    export_class(WindowSettings)
    export_class(Manager)
}
}
 
confirm_exports(ugdk_desktop)
