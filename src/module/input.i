%module ugdk_input

%include <module/export.swig>

%{

#include <ugdk/input/keys.h>
#include <ugdk/input/inputmanager.h>

%}

%import <module/base.i>
%import <module/math.i>

%newobject ugdk::input::InputManager::GetMousePosition();

%include <ugdk/input/keys.h>
%include <ugdk/input/inputmanager.h>

namespace ugdk {
namespace input {
    export_class(Key)
    export_class(MouseButton)
    export_class(InputManager)
}
}
 
confirm_exports(ugdk_input)
