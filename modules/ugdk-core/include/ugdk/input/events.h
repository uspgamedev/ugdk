#ifndef UGDK_INPUT_EVENTS_H_
#define UGDK_INPUT_EVENTS_H_

#include <ugdk/desktop.h>
#include <ugdk/input.h>
#include <ugdk/input/keycode.h>
#include <ugdk/input/scancode.h>
#include <ugdk/input/mousebutton.h>
#include <ugdk/input/joystickstatus.h>
#include <ugdk/math/integer2D.h>
#include <string>
#include <memory>

namespace ugdk {
namespace input {

struct KeyPressedEvent {
    Keycode keycode;
    Scancode scancode;
    Keymod modifiers;

    KeyPressedEvent(const Keycode& _keycode, const Scancode& _scancode,
                    const Keymod& _modifiers)
        : keycode(_keycode), scancode(_scancode), modifiers(_modifiers) {}
};
struct KeyHeldEvent {
    Keycode keycode;
    Scancode scancode;
    Keymod modifiers;
    
    KeyHeldEvent(const Keycode& _keycode, const Scancode& _scancode,
                    const Keymod& _modifiers)
        : keycode(_keycode), scancode(_scancode), modifiers(_modifiers) {}
};
struct KeyReleasedEvent {
    Keycode keycode;
    Scancode scancode;
    Keymod modifiers;
    
    KeyReleasedEvent(const Keycode& _keycode, const Scancode& _scancode,
                    const Keymod& _modifiers)
        : keycode(_keycode), scancode(_scancode), modifiers(_modifiers) {}
};

struct TextEditingEvent {
    const TextInput& input;

    TextEditingEvent(const TextInput& i) : input(i) {}
    TextEditingEvent& operator=(const TextEditingEvent&) = delete;
};

struct TextInputEvent {
    std::string text;

    TextInputEvent(const char* _text) : text(_text) {}
};

struct MouseMotionEvent {
    math::Integer2D position;
    math::Integer2D motion;
    std::weak_ptr<desktop::Window> window;
    
    MouseMotionEvent(const math::Integer2D& _position,
                     const math::Integer2D& _motion,
                     const std::weak_ptr<desktop::Window>& _window)
        : position(_position)
        , motion(_motion)
        , window(_window)
    {}
};
struct MouseButtonPressedEvent {
    math::Integer2D position;
    MouseButton button;

    MouseButtonPressedEvent(const math::Integer2D& _position, const MouseButton& _button)
        : position(_position), button(_button) {}
};
struct MouseButtonReleasedEvent {
    math::Integer2D position;
    MouseButton button;

    MouseButtonReleasedEvent(const math::Integer2D& _position, const MouseButton& _button)
        : position(_position), button(_button) {}
};
struct MouseWheelEvent {
    math::Integer2D scroll;

    MouseWheelEvent(const math::Integer2D& _scroll)
        : scroll(_scroll) {}
};

struct JoystickConnectedEvent {
    std::weak_ptr<Joystick> joystick;

    JoystickConnectedEvent(const std::weak_ptr<Joystick>& _joystick)
        : joystick(_joystick)
    {}
};
struct JoystickDisconnectedEvent {
    std::weak_ptr<Joystick> joystick;

    JoystickDisconnectedEvent(const std::weak_ptr<Joystick>& _joystick)
        : joystick(_joystick) {}
};
struct JoystickAxisEvent {
    std::weak_ptr<Joystick> joystick;
    int axis_id;
    AxisStatus axis_status;

    JoystickAxisEvent(const std::weak_ptr<Joystick>& _joystick,
                      int axis,
                      int16 status)
                      : joystick(_joystick)
                      , axis_id(axis)
                      , axis_status(status) {}
};
struct JoystickBallEvent {
    std::weak_ptr<Joystick> joystick;
    int ball_id;
    math::Integer2D ball_status;

    JoystickBallEvent(const std::weak_ptr<Joystick>& _joystick,
                      int ball,
                      int dx,
                      int dy)
                      : joystick(_joystick)
                      , ball_id(ball)
                      , ball_status(dx, dy) {}
};
struct JoystickHatEvent {
    std::weak_ptr<Joystick> joystick;
    int hat_id;
    HatStatus hat_status;

    JoystickHatEvent(const std::weak_ptr<Joystick>& _joystick,
                      int hat,
                      int status)
                      : joystick(_joystick)
                      , hat_id(hat)
                      , hat_status(status) {}
};
struct JoystickButtonPressedEvent {
    std::weak_ptr<Joystick> joystick;
    int button;

    JoystickButtonPressedEvent(const std::weak_ptr<Joystick>& _joystick,
                               int _button)
                               : joystick(_joystick)
                               , button(_button) {}
};
struct JoystickButtonReleasedEvent {
    std::weak_ptr<Joystick> joystick;
    int button;

    JoystickButtonReleasedEvent(const std::weak_ptr<Joystick>& _joystick,
                               int _button)
                               : joystick(_joystick)
                               , button(_button) {}
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_EVENTS_H_
