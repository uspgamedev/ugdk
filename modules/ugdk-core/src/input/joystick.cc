#include <ugdk/input/joystick.h>

#include "SDL_joystick.h"

namespace ugdk {
namespace input {

Joystick::Joystick(SDL_Joystick* joystick)
    : sdljoystick_(joystick)
{
    printf("Joystick CREATED %d\n", SDL_JoystickInstanceID(sdljoystick_));
}

Joystick::~Joystick() {
    printf("Joystick DELETED %d\n", SDL_JoystickInstanceID(sdljoystick_));
}

bool Joystick::IsConnected() const {
    return SDL_JoystickGetAttached(sdljoystick_) == SDL_TRUE;
}

} // namespace input
} // namespace ugdk
