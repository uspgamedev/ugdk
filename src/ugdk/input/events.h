#ifndef UGDK_INPUT_EVENTS_H_
#define UGDK_INPUT_EVENTS_H_

#include <ugdk/input.h>
#include <ugdk/input/keycode.h>
#include <ugdk/input/scancode.h>
#include <ugdk/input/mousebutton.h>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace input {

struct KeyPressedEvent {
    Keycode keycode;
    Scancode scancode;

    KeyPressedEvent(const Keycode& _keycode, const Scancode& _scancode)
        : keycode(_keycode), scancode(_scancode) {}
};
struct KeyHeldEvent {
    Keycode keycode;
    Scancode scancode;
    
    KeyHeldEvent(const Keycode& _keycode, const Scancode& _scancode)
        : keycode(_keycode), scancode(_scancode) {}
};
struct KeyReleasedEvent {
    Keycode keycode;
    Scancode scancode;
    
    KeyReleasedEvent(const Keycode& _keycode, const Scancode& _scancode)
        : keycode(_keycode), scancode(_scancode) {}
};

struct MouseMotionEvent {
    math::Integer2D motion;
    
    MouseMotionEvent(const math::Integer2D& _motion)
        : motion(_motion) {}
};
struct MouseButtonPressedEvent {
    MouseButton button;

    MouseButtonPressedEvent(const MouseButton& _button)
        : button(_button) {}
};
struct MouseButtonReleasedEvent {
    MouseButton button;

    MouseButtonReleasedEvent(const MouseButton& _button)
        : button(_button) {}
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_EVENTS_H_
