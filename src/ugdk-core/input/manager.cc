#include <ugdk/input/manager.h>

#include <system/sdleventhandler.h>
#include <ugdk/system/engine.h>

#include "SDL.h"

namespace ugdk {
namespace input {

// Both are defined in .cc because internal::SDLEventHandler is only known here.
Manager::Manager() {}
Manager::~Manager() {}

bool Manager::Initialize() {

    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0)
        return false;
    
    SDL_StopTextInput();

    system::RegisterSDLHandler(keyboard_.event_handler());
    system::RegisterSDLHandler(mouse_.event_handler());
    system::RegisterSDLHandler(text_input_.event_handler());

    return true;
}

void Manager::Release() {
    system::DeregisterSDLHandler(keyboard_.event_handler());
    system::DeregisterSDLHandler(mouse_.event_handler());
    system::DeregisterSDLHandler(text_input_.event_handler());
}

void Manager::Update() {
    keyboard_.Update();
    mouse_.Update();
}

}  // namespace input
}  // namespace ugdk


