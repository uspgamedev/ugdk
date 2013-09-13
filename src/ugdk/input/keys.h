#ifndef UGDK_INPUT_KEYS_H_
#define UGDK_INPUT_KEYS_H_

#include "SDL_keycode.h"
#include "SDL_mouse.h"

namespace ugdk {
namespace input {

typedef enum {
  M_BUTTON_LEFT = SDL_BUTTON_LEFT,
  M_BUTTON_MIDDLE = SDL_BUTTON_MIDDLE,
  M_BUTTON_RIGHT = SDL_BUTTON_RIGHT,
} MouseButton;

}  // namespace input
}  // namespace ugdk

#endif
