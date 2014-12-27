#include <ugdk/input/joystickstatus.h>

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

} // namespace input
} // namespace ugdk
