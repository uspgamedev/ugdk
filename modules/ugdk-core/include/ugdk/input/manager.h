#ifndef UGDK_INPUT_MANAGER_H_
#define UGDK_INPUT_MANAGER_H_

#include <ugdk/input/keyboard.h>
#include <ugdk/input/mouse.h>
#include <ugdk/input/joystick.h>
#include <ugdk/input/textinput.h>
#include <ugdk/structure/types.h>
#include <ugdk/system.h>

#include <memory>
#include <unordered_map>

namespace ugdk {
namespace input {

class Manager {
  public:
    Manager();
    ~Manager();

    /// Initializes input libraries.
    /** @return True if successful, false otherwise.
     */
    bool Initialize();
    void Release();

    /// A virtual keyboard that represents all physical keyboards.
    const Keyboard& keyboard() const { return keyboard_; }
    
    /// A virtual mouse that represents all physical mice.
    const Mouse& mouse() const { return mouse_; }
    
    /// Handles the IME.
    TextInput& text_input() { return text_input_; }

    void Update();

  private:
    std::shared_ptr<Joystick> CreateJoystick(int device);

    Keyboard keyboard_;
    Mouse mouse_;
    TextInput text_input_;
    std::unordered_map<int32, std::shared_ptr<Joystick>> joysticks_;
    std::unique_ptr<system::SDLEventHandler> event_handler_;

    friend class InputSDLEventHandler;
};

}  // namespace input
}  // namespace ugdk

#endif
