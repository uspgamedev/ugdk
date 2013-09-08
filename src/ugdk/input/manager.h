#ifndef UGDK_INPUT_MANAGER_H_
#define UGDK_INPUT_MANAGER_H_

#include <ugdk/input/keys.h>
#include <ugdk/internal.h>
#include <ugdk/math/vector2D.h>

#include <set>
#include <memory>

namespace ugdk {
namespace input {

#define BUFFER_SIZE 32

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

    void Update();
    ugdk::math::Vector2D GetMousePosition(void);
    void ShowCursor(bool toggle);
    bool KeyPressed(Key key);
    bool KeyReleased(Key key);
    bool KeyDown(Key key);
    bool KeyUp(Key key);
        
    bool MousePressed(MouseButton button);
    bool MouseReleased(MouseButton button);
    bool MouseDown(MouseButton button);
    bool MouseUp(MouseButton button);
	bool CheckSequence(Key* sequence, int size);

  private:
    std::set<Key> keystate_now_, keystate_last_;
    bool mousestate_now_[5], mousestate_last_[5];
	Key buffer_[BUFFER_SIZE];
	int buffer_end_;
    ugdk::math::Vector2D mouseposition_;
    std::unique_ptr<internal::SDLEventHandler> sdlevent_handler_;
    
    void UpdateDevices();
    
    friend class InputSDLEventHandler;
};

}  // namespace input
}  // namespace ugdk

#endif
