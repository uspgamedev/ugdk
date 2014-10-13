#include <ugdk/input/keycode.h>

#include "SDL_keyboard.h"

namespace ugdk {
namespace input {

Scancode CreateScancodeFromKeycode(const Keycode& keycode) {
    return Scancode(SDL_GetScancodeFromKey(static_cast< ::SDL_Keycode >(keycode)));
}

Keycode CreateKeycodeFromScancode(const Scancode& scancode) {
    return Keycode(SDL_GetKeyFromScancode(static_cast< ::SDL_Scancode >(scancode)));
}

} // namespace input
} // namespace ugdk
