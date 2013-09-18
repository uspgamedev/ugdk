#ifndef UGDK_INPUT_MANAGER_H_
#define UGDK_INPUT_MANAGER_H_

#include <ugdk/internal.h>
#include <ugdk/input/keyboard.h>
#include <ugdk/input/mouse.h>

#include <memory>

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

    const internal::SDLEventHandler* sdlevent_handler() { return sdlevent_handler_.get(); }

    /// A virtual keyboard that represents all physical keyboards.
    const Keyboard& keyboard() const { return keyboard_; }
    
    /// A virtual mouse that represents all physical mice.
    const Mouse& mouse() const { return mouse_; }

    void Update();

  private:
    std::unique_ptr<internal::SDLEventHandler> sdlevent_handler_;
    Keyboard keyboard_;
    Mouse mouse_;

    friend class InputSDLEventHandler;
};

}  // namespace input
}  // namespace ugdk

#endif
