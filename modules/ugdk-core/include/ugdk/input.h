#ifndef UGDK_INPUT_H_
#define UGDK_INPUT_H_

namespace ugdk {
namespace input {

#ifdef SWIG
#define ENUMCLASS struct
#else
#define ENUMCLASS enum class
#endif

class Manager;
class Keyboard;
class Mouse;
class TextInput;
class Joystick;
class AxisStatus;
class HatStatus;
ENUMCLASS Keycode;
ENUMCLASS Scancode;
ENUMCLASS Keymod;
ENUMCLASS MouseButton;

#undef ENUMCLASS

}  // namespace input
}  // namespace ugdk

#endif
