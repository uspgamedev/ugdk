#ifndef UGDK_INPUT_KEYBOARD_H_
#define UGDK_INPUT_KEYBOARD_H_

#include <ugdk/system.h>
#include <ugdk/input/scancode.h>
#include <ugdk/input/keycode.h>
#include <set>
#include <memory>

namespace ugdk {
namespace input {

class Keyboard {
  public:
    bool IsDown(const Scancode& scancode) const;
    bool IsDown(const Keycode& keycode) const;
    bool IsUp(const Scancode& scancode) const;
    bool IsUp(const Keycode& keycode) const;

    bool IsPressed(const Scancode& scancode) const;
    bool IsPressed(const Keycode& keycode) const;
    bool IsReleased(const Scancode& scancode) const;
    bool IsReleased(const Keycode& keycode) const;

  private:
    Keyboard();
    ~Keyboard();

    void Update();
    const system::SDLEventHandler* event_handler();

    std::unique_ptr<system::SDLEventHandler> event_handler_;
    std::set<Scancode> keystate_, keystate_previous_;

    friend class Manager;
    friend class KeyboardInputSDLEventHandler;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_KEYBOARD_H_
