#ifndef UGDK_INPUT_INPUTMANAGER_H_
#define UGDK_INPUT_INPUTMANAGER_H_

#include <ugdk/config/config.h>
#include "SDL.h"
#include <ugdk/math/vector2D.h>
#include <ugdk/input/keys.h>

namespace ugdk {
namespace input {

#define BUFFER_SIZE 32
#define INPUT_MANAGER() (ugdk::Engine::reference()->input_manager())

class InputManager {
  public:
    // Construtores e destrutores
    InputManager();
    ~InputManager();

    // Member functions e
    void Update(float);
    Vector2D GetMousePosition(void);
    void ShowCursor(bool toggle);
    bool KeyPressed(Key key);
    bool KeyDown(Key key);
    bool KeyUp(Key key);
        
    bool MousePressed(MouseButton button);
    bool MouseDown(MouseButton button);
    bool MouseUp(MouseButton button);
	bool CheckSequence(Key* sequence, int size);
    
    void SimulateKeyPress(Key key);
    void SimulateKeyRelease(Key key);

  private:
    int kbsize_;
    bool *keystate_now_, *keystate_last_;
    bool mousestate_now_[5], mousestate_last_[5];
	Key buffer_[BUFFER_SIZE];
	int buffer_end_;
    Vector2D mouseposition_;
    
    void UpdateDevices();
};

}  // namespace input
}  // namespace ugdk

#endif
