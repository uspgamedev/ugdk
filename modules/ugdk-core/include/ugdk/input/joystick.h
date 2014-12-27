#ifndef UGDK_INPUT_JOYSTICK_H_
#define UGDK_INPUT_JOYSTICK_H_

#include <ugdk/system.h>
#include <ugdk/input/joystickstatus.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/system/eventhandler.h>

#include <set>
#include <memory>

struct _SDL_Joystick;

namespace ugdk {
namespace input {

class Joystick {
  public:
    ~Joystick();

    /// Checks if the joystick is currently connected.
    bool IsConnected() const;

    /// Get the number of general axis controls.
    int NumAxes() const;

    /// Get the number of trackballs.
    /** Trackballs have only relative motion events associated
    *   with them and their state cannot be polled.
    */
    int NumTrackballs() const;

    /// Get the number of POV hats.
    int NumHats() const;

    /// Get the number of buttons.
    int NumButtons() const;

    /// Checks if the given button is currently being held.
    bool IsDown(int button) const;

    /// Checks if there's currently no interaction with the given button.
    bool IsUp(int button) const;

    /// Get the ball axis changes since last frame.
    math::Integer2D GetBallDelta(int ballid) const;

    /// Get the current state of a POV hat.
    HatStatus GetHatStatus(int hat) const;

    /// Get the current state of an axis control.
    AxisStatus GetAxisStatus(int axis) const;

    /// An event handler for events that matching this joystick.
    system::EventHandler& event_handler() { return event_handler_; }

  private:
    Joystick(_SDL_Joystick*);

    _SDL_Joystick* sdljoystick_;
    system::EventHandler event_handler_;

    friend class Manager;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_JOYSTICK_H_
