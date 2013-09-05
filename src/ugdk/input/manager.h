#ifndef UGDK_INPUT_MANAGER_H_
#define UGDK_INPUT_MANAGER_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/input/keys.h>
#include <set>

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
    
    void SimulateKeyPress(Key key);
    void SimulateKeyRelease(Key key);

  private:
    std::set<Key> keystate_now_, keystate_last_;
    bool mousestate_now_[5], mousestate_last_[5];
	Key buffer_[BUFFER_SIZE];
	int buffer_end_;
    ugdk::math::Vector2D mouseposition_;
    
    void UpdateDevices();
};

}  // namespace input
}  // namespace ugdk

#endif
