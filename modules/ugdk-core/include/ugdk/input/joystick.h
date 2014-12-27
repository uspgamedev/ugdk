#ifndef UGDK_INPUT_JOYSTICK_H_
#define UGDK_INPUT_JOYSTICK_H_

#include <ugdk/system.h>
#include <ugdk/input/mousebutton.h>
#include <ugdk/math/integer2D.h>
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

  private:
    Joystick(_SDL_Joystick*);

    _SDL_Joystick* sdljoystick_;

    friend class Manager;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_JOYSTICK_H_
