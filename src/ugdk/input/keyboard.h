#ifndef UGDK_INPUT_KEYBOARD_H_
#define UGDK_INPUT_KEYBOARD_H_

#include <ugdk/input/scancode.h>
#include <ugdk/input/keycode.h>
#include <set>

namespace ugdk {
namespace input {

class Keyboard {
  public:
    bool IsDown(const Scancode& scancode) const {
        return keystate_.count(scancode) > 0;
    }
    bool IsDown(const Keycode& keycode) const {
        return IsDown(CreateScancodeFromKeycode(keycode));
    }
    bool IsUp(const Scancode& scancode) const {
        return keystate_.count(scancode) == 0;
    }
    bool IsUp(const Keycode& keycode) const {
        return IsUp(CreateScancodeFromKeycode(keycode));
    }

    bool IsPressed(const Scancode& scancode) const {
        return IsDown(scancode) && keystate_previous_.count(scancode) == 0;
    }
    bool IsPressed(const Keycode& keycode) const {
        return IsPressed(CreateScancodeFromKeycode(keycode));
    }
    bool IsReleased(const Scancode& scancode) const {
        return IsUp(scancode) && keystate_previous_.count(scancode) > 0;
    }
    bool IsReleased(const Keycode& keycode) const {
        return IsReleased(CreateScancodeFromKeycode(keycode));
    }
  private:
    Keyboard() {}

    std::set<Scancode> keystate_, keystate_previous_;

    friend class Manager;
    friend class InputSDLEventHandler;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_KEYBOARD_H_
