#include <ugdk/input/joystick.h>

#include <ugdk/system/exceptions.h>

#include "SDL_joystick.h"

namespace ugdk {
namespace input {

AxisStatus::AxisStatus(int16 raw)
    : raw_(raw)
{}

int16 AxisStatus::raw() const { return raw_; }

int16 AxisStatus::MaximumValue() const {
    return 32767;
}

int16 AxisStatus::MinimumValue() const {
    return -32768;
}

double AxisStatus::Percentage() const {
    if (raw_ < 0) {
        return -raw_ / static_cast<double>(MinimumValue());
    } else {
        return raw_ / static_cast<double>(MaximumValue());
    }
}

HatStatus::HatStatus(int raw)
    : raw_(raw)
{}

int HatStatus::raw() const { return raw_; }

bool HatStatus::IsCentered() const {
    return raw_ == SDL_HAT_CENTERED;
}
bool HatStatus::IsUp() const {
    return raw_ == SDL_HAT_UP;
}
bool HatStatus::IsRight() const {
    return raw_ == SDL_HAT_RIGHT;
}
bool HatStatus::IsDown() const {
    return raw_ == SDL_HAT_DOWN;
}
bool HatStatus::IsLeft() const {
    return raw_ == SDL_HAT_LEFT;
}
bool HatStatus::IsRightUp() const {
    return raw_ == SDL_HAT_RIGHTUP;
}
bool HatStatus::IsRightDown() const {
    return raw_ == SDL_HAT_RIGHTDOWN;
}
bool HatStatus::IsLeftUp() const {
    return raw_ == SDL_HAT_LEFTUP;
}
bool HatStatus::IsLeftDown() const {
    return raw_ == SDL_HAT_LEFTDOWN;
}

bool HatStatus::HasUp() const {
    return (raw_ & SDL_HAT_UP) != 0;
}
bool HatStatus::HasRight() const {
    return (raw_ & SDL_HAT_RIGHT) != 0;
}
bool HatStatus::HasDown() const {
    return (raw_ & SDL_HAT_DOWN) != 0;
}
bool HatStatus::HasLeft() const {
    return (raw_ & SDL_HAT_LEFT) != 0;
}

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

int Joystick::NumAxes() const {
	return SDL_JoystickNumAxes(sdljoystick_);
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
