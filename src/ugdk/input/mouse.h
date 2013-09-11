#ifndef UGDK_INPUT_MOUSE_H_
#define UGDK_INPUT_MOUSE_H_

#include <ugdk/math/integer2D.h>

namespace ugdk {
namespace input {

class Mouse {
  public:
    const math::Integer2D& position() const { return position_; }

  private:
    Mouse() {}

    math::Integer2D position_;

    friend class Manager;
    friend class InputSDLEventHandler;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_KEYBOARD_H_
