#ifndef HORUSEYE_FRAMEWORK_INPUTMANAGER_H_
#define HORUSEYE_FRAMEWORK_INPUTMANAGER_H_

#include <SDL/SDL.h>
#include "vector2D.h"
#include "keys.h"

namespace ugdk {

#define BUFFER_SIZE 32

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

}
#endif
