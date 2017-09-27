#include <ugdk/input/joystick.h>

#include <ugdk/system/exceptions.h>

#include "SDL_joystick.h"

namespace ugdk {
namespace input {

using std::string;

Joystick::Joystick(SDL_Joystick* joystick)
    : sdljoystick_(joystick)
{}

Joystick::~Joystick() {}

bool Joystick::IsConnected() const {
    return SDL_JoystickGetAttached(sdljoystick_) == SDL_TRUE;
}

int Joystick::NumAxes() const {
	return SDL_JoystickNumAxes(sdljoystick_);
}

string Joystick::name() const {
    return SDL_JoystickName(sdljoystick_);
}

int Joystick::NumTrackballs() const {
	return SDL_JoystickNumBalls(sdljoystick_);
}

int Joystick::NumHats() const {
	return SDL_JoystickNumHats(sdljoystick_);
}

int Joystick::NumButtons() const {
	return SDL_JoystickNumButtons(sdljoystick_);
}

bool Joystick::IsDown(int button) const {
    return SDL_JoystickGetButton(sdljoystick_, button) > 0;
}

bool Joystick::IsUp(int button) const {
    return SDL_JoystickGetButton(sdljoystick_, button) == 0;
}

math::Integer2D Joystick::GetBallDelta(int ballid) const {
    math::Integer2D result;
    if (0 != SDL_JoystickGetBall(sdljoystick_, ballid, &result.x, &result.y)) {
        throw system::InvalidOperation("Invalid ballid");
    }
    return result;
}

HatStatus Joystick::GetHatStatus(int hat) const {
    return HatStatus(SDL_JoystickGetHat(sdljoystick_, hat));
}

AxisStatus Joystick::GetAxisStatus(int axis) const {
    return AxisStatus(SDL_JoystickGetAxis(sdljoystick_, axis));
}

} // namespace input
} // namespace ugdk
