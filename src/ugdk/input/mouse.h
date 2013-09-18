#ifndef UGDK_INPUT_MOUSE_H_
#define UGDK_INPUT_MOUSE_H_

#include <ugdk/internal.h>
#include <ugdk/input/mousebutton.h>
#include <ugdk/math/integer2D.h>
#include <set>
#include <memory>

namespace ugdk {
namespace input {

class Mouse {
  public:
    const math::Integer2D& position() const { return position_; }

    bool IsDown(const MouseButton&) const;
    bool IsUp(const MouseButton&) const;

    bool IsPressed(const MouseButton&) const;
    bool IsReleased(const MouseButton&) const;


  private:
    Mouse();
    ~Mouse();
    
    void Update();
    const internal::SDLEventHandler* event_handler();

    std::unique_ptr<internal::SDLEventHandler> event_handler_;
    math::Integer2D position_;
    std::set<MouseButton> state_, state_previous_;

    friend class Manager;
    friend class MouseInputSDLEventHandler;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_KEYBOARD_H_
