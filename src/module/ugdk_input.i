%module ugdk_input

%include <module/util_ugdk.swig>

%{

#include <ugdk/input/keyboard.h>
#include <ugdk/input/mouse.h>
#include <ugdk/input/textinput.h>
#include <ugdk/input/manager.h>
#include <ugdk/input/module.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_system") <ugdk/structure/types.h>
%include <ugdk/internal.h>

%import(module="ugdk_input_scancode") <ugdk/input/scancode.h>
%import(module="ugdk_input_keycode") <ugdk/input/keycode.h>
%import(module="ugdk_input_mousebutton") <ugdk/input/mousebutton.h>

// SWIG can't differ from Keycode/Scancode overload.
%ignore ugdk::input::Keyboard::IsDown(const Keycode& keycode) const;
%ignore ugdk::input::Keyboard::IsUp(const Keycode& keycode) const;
%ignore ugdk::input::Keyboard::IsPressed(const Keycode& keycode) const;
%ignore ugdk::input::Keyboard::IsReleased(const Keycode& keycode) const;
//

%include <ugdk/input/keyboard.h>
%include <ugdk/input/mouse.h>
%include <ugdk/input/textinput.h>
%include <ugdk/input/manager.h>
%include <ugdk/input/module.h>

namespace ugdk {
namespace input {
    export_class(Manager)
    export_class(Keyboard)
    export_class(TextInput)
    export_class(Mouse)
}
}
 
confirm_exports(ugdk_input)
