%module ugdk_input

%include <module/util_ugdk.swig>

%{

#include <ugdk/input/keyboard.h>
#include <ugdk/input/mouse.h>
#include <ugdk/input/manager.h>
#include <ugdk/input/module.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_system") <ugdk/structure/types.h>
%include <ugdk/internal.h>
 
namespace ugdk {
namespace input {
struct Scancode {};
struct Keycode {};
struct Keymod {};
struct MouseButton {};
}
}
%include <ugdk/input/keyboard.h>
%include <ugdk/input/mouse.h>
%include <ugdk/input/manager.h>
%include <ugdk/input/module.h>

namespace ugdk {
namespace input {
    export_class(Manager)
    export_class(Keyboard)
    export_class(Mouse)
}
}
 
confirm_exports(ugdk_input)
