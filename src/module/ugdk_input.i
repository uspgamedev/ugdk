%module ugdk_input

%include <module/export.swig>

%{

#include <ugdk/input/keys.h>
#include <ugdk/input/manager.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_base") <ugdk/common/types.h>

%newobject ugdk::input::Manager::GetMousePosition();

%include <ugdk/input/keys.h>
%include <ugdk/input/manager.h>

namespace ugdk {
namespace input {
    export_class(Key)
    export_class(MouseButton)
    export_class(Manager)
}
}
 
confirm_exports(ugdk_input)
