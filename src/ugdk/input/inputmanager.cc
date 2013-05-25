#include <algorithm>
#include "SDL.h"
#include <ugdk/input/inputmanager.h>

namespace ugdk {
namespace input {

using math::Vector2D;

InputManager::InputManager() {
    SDL_GetKeyState(&kbsize_);
    keystate_now_ = new bool[kbsize_];
    keystate_last_ = new bool[kbsize_];
	buffer_end_ = 0;
	for(int i = 0; i < BUFFER_SIZE; i++)
		buffer_[i] = K_UNKNOWN;
    
    std::fill(mousestate_now_, mousestate_now_+5, false);
    std::fill(mousestate_last_, mousestate_last_+5, false);
    std::fill(keystate_now_, keystate_now_+kbsize_, false);
    std::fill(keystate_last_, keystate_last_+kbsize_, false);
    
    Update(1.3);
}

InputManager::~InputManager() {
    delete[] keystate_now_;
    delete[] keystate_last_;
}

void InputManager::Update(double delta_t) {
    int i;
    
    // bufferiza teclado
    for(i = 0; i < kbsize_; i++)
      keystate_last_[i] = keystate_now_[i];
      
    // bufferiza botoes do mouse
    for(i = 0; i < 5; i++)
      mousestate_last_[i] = mousestate_now_[i];
      
    // atualiza dispositivos
    UpdateDevices();
}

Vector2D InputManager::GetMousePosition(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return Vector2D((double) x, (double) y);
}

void InputManager::ShowCursor(bool toggle) {
    SDL_ShowCursor((int) toggle);
}

bool InputManager::KeyPressed(Key key) {
    return (keystate_now_[key] && !keystate_last_[key]);
}

bool InputManager::KeyReleased(Key key) {
    return (!keystate_now_[key] && keystate_last_[key]);
}

bool InputManager::KeyDown(Key key) {
    return keystate_now_[key];
}

bool InputManager::KeyUp(Key key) {
    return !keystate_now_[key];
}

bool InputManager::CheckSequence(Key* sequence, int size) {
	int iterator = buffer_end_ - 1;
	for(int seq = size - 1; seq >= 0; seq--) {
		if(iterator < 0) iterator += BUFFER_SIZE;
		if(buffer_[iterator] != sequence[seq])
			return false;
		iterator--;
	}
	return true;
}

bool InputManager::MousePressed(MouseButton button) {
  return (mousestate_now_[button] && !mousestate_last_[button]);
}

bool InputManager::MouseReleased(MouseButton button) {
    return (!mousestate_now_[button] && mousestate_last_[button]);
}

bool InputManager::MouseDown(MouseButton button) {
    return mousestate_now_[button];
}

bool InputManager::MouseUp(MouseButton button) {
    return !mousestate_now_[button];
}

void InputManager::UpdateDevices() {
    int i;
    
    // atualiza mouse
    for(i = 0; i < 5; i++) {
        if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(i))
            mousestate_now_[i] = true;
        else
            mousestate_now_[i] = false;
    }
}

void InputManager::SimulateKeyRelease(Key key) {
    int k = (int)key;
    if(k >= 0 && k < kbsize_)
        keystate_now_[k] = false;
}
 
void InputManager::SimulateKeyPress(Key key) {
    int k = (int)key;
	if(k >= 0 && k < kbsize_) {
        keystate_now_[k] = true;
		buffer_[buffer_end_] = key;
		buffer_end_ = (buffer_end_ + 1) % BUFFER_SIZE;
	}
}

}  // namespace input
}  // namespace ugdk


