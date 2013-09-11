#ifndef UGDK_INPUT_EVENTS_H_
#define UGDK_INPUT_EVENTS_H_

#include <ugdk/input.h>
#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace input {

struct KeyPressedEvent {};
struct KeyReleasedEvent {};

struct MouseMotionEvent {
    const Mouse& mouse;
    math::Integer2D motion;
    
    MouseMotionEvent(const Mouse& _mouse, const math::Integer2D& _motion)
        : mouse(_mouse), motion(_motion) {}
};
struct MouseButtonPressedEvent {};
struct MouseButtonReleasedEvent {};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_EVENTS_H_
