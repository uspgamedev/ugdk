#ifndef UGDK_INPUT_KEYBOARD_H_
#define UGDK_INPUT_KEYBOARD_H_

#include <ugdk/input/keys.h>
#include <set>

namespace ugdk {
namespace input {

class Keyboard {
  public:
    bool IsDown(Key key) const { return keystate_.count(key) > 0; }
    bool IsUp(Key key) const { return keystate_.count(key) == 0; }

  private:
    Keyboard() {}

    std::set<Key> keystate_;

    friend class Manager;
    friend class InputSDLEventHandler;
};

} // namespace input
} // namespace ugdk

#endif // UGDK_INPUT_KEYBOARD_H_
