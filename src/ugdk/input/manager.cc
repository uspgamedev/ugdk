#include <ugdk/input/manager.h>

#include <algorithm>
#include <utility>
#include <map>
#include <functional>
#include "SDL.h"

#include <ugdk/internal/sdleventhandler.h>

namespace ugdk {
namespace input {

using math::Vector2D;

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class InputSDLEventHandler;
typedef std::map<::Uint32, void (InputSDLEventHandler::*)(const ::SDL_Event&) const> InputCallback;
class InputSDLEventHandler : public internal::SDLEventHandler {
public:
    InputSDLEventHandler(Manager* manager) : manager_(manager) {}

    bool CanHandle(const ::SDL_Event& sdlevent) const {
        /* We can handle the following:
        * Keyboard events
        * Mouse events
        * Joystick events
        * Game controller events
        * Touch events
        * Gesture events
        */
        return SDL_KEYDOWN <= sdlevent.type && sdlevent.type <= SDL_MULTIGESTURE;
    }

    void Handle(const ::SDL_Event& sdlevent) const {
        auto handler = event_methods_.find(sdlevent.type);
        if(handler != event_methods_.end()) {
            CALL_MEMBER_FN(*this,handler->second)(sdlevent);
        }
    }

    void KeyDownHandler(const ::SDL_Event& sdlevent) const {
        printf("Key Pressed: '%s'\n", SDL_GetKeyName(sdlevent.key.keysym.sym));
        manager_->keystate_now_.insert(static_cast<input::Key>(sdlevent.key.keysym.sym));
    }
    
    void KeyUpHandler(const ::SDL_Event& sdlevent) const {
        manager_->keystate_now_.erase(static_cast<input::Key>(sdlevent.key.keysym.sym));
    }

  private:
    Manager* manager_;
    static InputCallback event_methods_;
};

InputCallback InputSDLEventHandler::event_methods_ = [] {
    InputCallback result;
    result[SDL_KEYDOWN] = &InputSDLEventHandler::KeyDownHandler;
    result[SDL_KEYUP] = &InputSDLEventHandler::KeyUpHandler;
    return result;
}();

Manager::Manager() {}

Manager::~Manager() {}

bool Manager::Initialize() {

    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
        return false;

    sdlevent_handler_.reset(new InputSDLEventHandler(this));

	buffer_end_ = 0;
	for(int i = 0; i < BUFFER_SIZE; i++)
		buffer_[i] = K_UNKNOWN;
    
    std::fill(mousestate_now_, mousestate_now_+5, false);
    std::fill(mousestate_last_, mousestate_last_+5, false);
    
    Update();
    return true;
}

void Manager::Release() {
}

void Manager::Update() {
    int i;
    
    // bufferiza teclado
    keystate_last_ = keystate_now_;
      
    // bufferiza botoes do mouse
    for(i = 0; i < 5; i++)
      mousestate_last_[i] = mousestate_now_[i];
      
    // atualiza dispositivos
    UpdateDevices();
}

Vector2D Manager::GetMousePosition(void) {
    int x, y;
    SDL_GetMouseState(&x, &y);
    return Vector2D((double) x, (double) y);
}

void Manager::ShowCursor(bool toggle) {
    SDL_ShowCursor((int) toggle);
}

bool Manager::KeyPressed(Key key) {
    return (keystate_now_.count(key) > 0 && keystate_last_.count(key) == 0);
}

bool Manager::KeyReleased(Key key) {
    return (keystate_now_.count(key) == 0 && keystate_last_.count(key) > 0);
}

bool Manager::KeyDown(Key key) {
    return keystate_now_.count(key) > 0;
}

bool Manager::KeyUp(Key key) {
    return keystate_now_.count(key) == 0;
}

bool Manager::CheckSequence(Key* sequence, int size) {
	int iterator = buffer_end_ - 1;
	for(int seq = size - 1; seq >= 0; seq--) {
		if(iterator < 0) iterator += BUFFER_SIZE;
		if(buffer_[iterator] != sequence[seq])
			return false;
		iterator--;
	}
	return true;
}

bool Manager::MousePressed(MouseButton button) {
  return (mousestate_now_[button] && !mousestate_last_[button]);
}

bool Manager::MouseReleased(MouseButton button) {
    return (!mousestate_now_[button] && mousestate_last_[button]);
}

bool Manager::MouseDown(MouseButton button) {
    return mousestate_now_[button];
}

bool Manager::MouseUp(MouseButton button) {
    return !mousestate_now_[button];
}

void Manager::UpdateDevices() {
    int i;
    
    // atualiza mouse
    for(i = 0; i < 5; i++) {
        if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(i))
            mousestate_now_[i] = true;
        else
            mousestate_now_[i] = false;
    }
}

}  // namespace input
}  // namespace ugdk


