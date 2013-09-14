#include <ugdk/input/manager.h>

#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/system/engine.h>
#include <ugdk/util/utf8.h>

#include <algorithm>
#include <utility>
#include <map>
#include <functional>

#include "SDL.h"

namespace ugdk {
namespace input {

using math::Vector2D;

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

class InputSDLEventHandler;
typedef std::map< ::Uint32, void (InputSDLEventHandler::*)(const ::SDL_Event&) const> InputCallback;
class InputSDLEventHandler : public internal::SDLEventHandler {
public:
    InputSDLEventHandler(Manager& manager) : manager_(manager) {}

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
        } else {
            printf("UGDK::Input -- Unhandled event of type 0x%X\n", sdlevent.type);
        }
    }

    void KeyDownHandler(const ::SDL_Event& sdlevent) const {
        manager_.keyboard_.event_handler()->Handle(sdlevent);
    }
    
    void KeyUpHandler(const ::SDL_Event& sdlevent) const {
        manager_.keyboard_.event_handler()->Handle(sdlevent);
    }

    void TextInputHandler(const ::SDL_Event& sdlevent) const {
        //printf("TextInput: %s\n", sdlevent.text.text);
    }

    void MouseMotionHandler(const ::SDL_Event& sdlevent) const {
        manager_.mouse_.position_.x = sdlevent.motion.x;
        manager_.mouse_.position_.y = sdlevent.motion.y;
        system::CurrentScene()->event_handler().RaiseEvent(MouseMotionEvent(
            manager_.mouse_,
            math::Integer2D(sdlevent.motion.xrel, sdlevent.motion.yrel)
        ));
    }

    void MouseButtonDownHandler(const ::SDL_Event& sdlevent) const {
        system::CurrentScene()->event_handler().RaiseEvent(MouseButtonPressedEvent());
    }

    void MouseButtonUpHandler(const ::SDL_Event& sdlevent) const {
        system::CurrentScene()->event_handler().RaiseEvent(MouseButtonReleasedEvent());
    }

  private:
    Manager& manager_;
    static InputCallback event_methods_;

    void operator=(const InputSDLEventHandler&);
};

InputCallback InputSDLEventHandler::event_methods_ = [] {
    InputCallback result;
    result[SDL_KEYDOWN] = &InputSDLEventHandler::KeyDownHandler;
    result[SDL_KEYUP] = &InputSDLEventHandler::KeyUpHandler;
    result[SDL_TEXTINPUT] = &InputSDLEventHandler::TextInputHandler;
    result[SDL_MOUSEMOTION] = &InputSDLEventHandler::MouseMotionHandler;
    result[SDL_MOUSEBUTTONDOWN] = &InputSDLEventHandler::MouseButtonDownHandler;
    result[SDL_MOUSEBUTTONUP] = &InputSDLEventHandler::MouseButtonUpHandler;
    return result;
}();

Manager::Manager() {}

Manager::~Manager() {}

bool Manager::Initialize() {

    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
        return false;
    
    //SDL_StopTextInput();

    sdlevent_handler_.reset(new InputSDLEventHandler(*this));

    return true;
}

void Manager::Release() {
}

void Manager::Update() {
    keyboard_.keystate_previous_ = keyboard_.keystate_;
}

void Manager::ShowCursor(bool toggle) {
    SDL_ShowCursor((int) toggle);
}

}  // namespace input
}  // namespace ugdk


