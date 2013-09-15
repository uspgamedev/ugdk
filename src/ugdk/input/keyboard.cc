#include <ugdk/input/keyboard.h>

#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/system/engine.h>

namespace ugdk {
namespace input {

class KeyboardInputSDLEventHandler : public internal::SDLEventHandler {
  public:
    KeyboardInputSDLEventHandler(Keyboard& keyboard) : keyboard_(keyboard) {}

    bool CanHandle(const ::SDL_Event& sdlevent) const {
        return sdlevent.type == SDL_KEYDOWN || sdlevent.type == SDL_KEYUP;
    }

    void Handle(const ::SDL_Event& sdlevent) const {
        if(sdlevent.type == SDL_KEYDOWN)
            KeyDownHandler(sdlevent);
        else if(sdlevent.type == SDL_KEYUP)
            KeyUpHandler(sdlevent);
        else
            assert(false);
    }

    void KeyDownHandler(const ::SDL_Event& sdlevent) const {
        keyboard_.keystate_.insert(Scancode(sdlevent.key.keysym.scancode));
        if(sdlevent.key.repeat == 0)
            system::CurrentScene()->event_handler().ScheduleEvent(KeyPressedEvent(
                Keycode(sdlevent.key.keysym.sym),
                Scancode(sdlevent.key.keysym.scancode),
                Keymod(sdlevent.key.keysym.mod)));
        else
            system::CurrentScene()->event_handler().ScheduleEvent(KeyHeldEvent(
                Keycode(sdlevent.key.keysym.sym),
                Scancode(sdlevent.key.keysym.scancode),
                Keymod(sdlevent.key.keysym.mod)));
        printf("Key down, keycode: 0x%X -- scancode: %d -- repeat: %d\n", sdlevent.key.keysym.sym, sdlevent.key.keysym.scancode, sdlevent.key.repeat);
    }
    
    void KeyUpHandler(const ::SDL_Event& sdlevent) const {
        keyboard_.keystate_.erase(Scancode(sdlevent.key.keysym.scancode));
        system::CurrentScene()->event_handler().ScheduleEvent(KeyReleasedEvent(
                Keycode(sdlevent.key.keysym.sym),
                Scancode(sdlevent.key.keysym.scancode),
                Keymod(sdlevent.key.keysym.mod)));
        printf("Key up, keycode: 0x%X -- scancode: %d\n", sdlevent.key.keysym.sym, sdlevent.key.keysym.scancode);
    }


  private:
    void operator=(const KeyboardInputSDLEventHandler&);

    Keyboard& keyboard_;
};

Keyboard::Keyboard() : event_handler_(new KeyboardInputSDLEventHandler(*this)) {}

Keyboard::~Keyboard() {}

bool Keyboard::IsDown(const Scancode& scancode) const {
    return keystate_.count(scancode) > 0;
}
bool Keyboard::IsDown(const Keycode& keycode) const {
    return IsDown(CreateScancodeFromKeycode(keycode));
}
bool Keyboard::IsUp(const Scancode& scancode) const {
    return keystate_.count(scancode) == 0;
}
bool Keyboard::IsUp(const Keycode& keycode) const {
    return IsUp(CreateScancodeFromKeycode(keycode));
}

bool Keyboard::IsPressed(const Scancode& scancode) const {
    return IsDown(scancode) && keystate_previous_.count(scancode) == 0;
}
bool Keyboard::IsPressed(const Keycode& keycode) const {
    return IsPressed(CreateScancodeFromKeycode(keycode));
}
bool Keyboard::IsReleased(const Scancode& scancode) const {
    return IsUp(scancode) && keystate_previous_.count(scancode) > 0;
}
bool Keyboard::IsReleased(const Keycode& keycode) const {
    return IsReleased(CreateScancodeFromKeycode(keycode));
}
    
const internal::SDLEventHandler* Keyboard::event_handler() {
    return event_handler_.get();
}

} // namespace input
} // namespace ugdk
