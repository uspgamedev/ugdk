#include <ugdk/input/manager.h>

#include <ugdk/internal/sdleventhandler.h>
#include <ugdk/internal/sdleventdelegatorhandler.h>

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

    internal::SDLEventDelegatorHandler* handler = new internal::SDLEventDelegatorHandler;
    handler->AddHandler(keyboard_.event_handler());
    handler->AddHandler(mouse_.event_handler());
    sdlevent_handler_.reset(handler);

    return true;
}

void Manager::Release() {}

void Manager::Update() {
    keyboard_.Update();
    mouse_.Update();
}

}  // namespace input
}  // namespace ugdk


