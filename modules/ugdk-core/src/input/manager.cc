#include <ugdk/input/manager.h>

#include <system/sdleventhandler.h>
#include <ugdk/system/engine.h>
#include <ugdk/action/scene.h>
#include <ugdk/debug/log.h>
#include <ugdk/input/events.h>

#include "SDL.h"

namespace ugdk {
namespace input {

class InputSDLEventHandler : public system::SDLEventHandler {
  public:
    InputSDLEventHandler(Manager& manager) : manager_(manager) {}

    std::unordered_set<Uint32> TypesHandled() const override {
        return{ SDL_JOYDEVICEADDED, SDL_JOYDEVICEREMOVED,
            SDL_JOYAXISMOTION, SDL_JOYBALLMOTION, SDL_JOYHATMOTION,
            SDL_JOYBUTTONDOWN, SDL_JOYBUTTONUP };
    }

    void Handle(const ::SDL_Event& sdlevent) const override {
        std::shared_ptr<Joystick> joystick;
        switch (sdlevent.type) {
            case SDL_JOYDEVICEADDED:
                joystick = manager_.CreateJoystick(sdlevent.jdevice.which);
                if (joystick) {
                    system::EventHandler::RaiseGlobalEvent(JoystickConnectedEvent(joystick));
                }
                break;

            case SDL_JOYDEVICEREMOVED:
                joystick = manager_.joysticks_[sdlevent.jdevice.which];
                if (joystick)
                    joystick->event_handler().RaiseEvent(JoystickDisconnectedEvent(joystick));
                manager_.joysticks_.erase(sdlevent.jdevice.which);
                break;

            case SDL_JOYAXISMOTION:
                joystick = manager_.joysticks_.at(sdlevent.jaxis.which);
                joystick->event_handler().RaiseEvent(JoystickAxisEvent(
                    joystick,
                    sdlevent.jaxis.axis,
                    sdlevent.jaxis.value));
                break;

            case SDL_JOYBALLMOTION:
                joystick = manager_.joysticks_.at(sdlevent.jaxis.which);
                joystick->event_handler().RaiseEvent(JoystickBallEvent(
                    joystick,
                    sdlevent.jball.ball,
                    sdlevent.jball.xrel,
                    sdlevent.jball.yrel));
                break;

            case SDL_JOYHATMOTION:
                joystick = manager_.joysticks_.at(sdlevent.jaxis.which);
                joystick->event_handler().RaiseEvent(JoystickHatEvent(
                    joystick,
                    sdlevent.jhat.hat,
                    sdlevent.jhat.value));
                break;

            case SDL_JOYBUTTONDOWN:
                joystick = manager_.joysticks_.at(sdlevent.jaxis.which);
                joystick->event_handler().RaiseEvent(JoystickButtonPressedEvent(
                    joystick,
                    sdlevent.jbutton.button));
                break;

            case SDL_JOYBUTTONUP:
                joystick = manager_.joysticks_.at(sdlevent.jaxis.which);
                joystick->event_handler().RaiseEvent(JoystickButtonReleasedEvent(
                    joystick,
                    sdlevent.jbutton.button));
                break;

            default:
                break;
        }
    }

  private:
    void operator=(const InputSDLEventHandler&) = delete;

    Manager& manager_;
};

// Both are defined in .cc because internal::SDLEventHandler is only known here.
Manager::Manager() : event_handler_(new InputSDLEventHandler(*this)) {}
Manager::~Manager() {}

bool Manager::Initialize() {

    if(SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
        return false;
    
    SDL_StopTextInput();

    system::RegisterSDLHandler(event_handler_.get());
    system::RegisterSDLHandler(keyboard_.event_handler());
    system::RegisterSDLHandler(mouse_.event_handler());
    system::RegisterSDLHandler(text_input_.event_handler());

    return true;
}

void Manager::Release() {
    system::UnregisterSDLHandler(event_handler_.get());
    system::UnregisterSDLHandler(keyboard_.event_handler());
    system::UnregisterSDLHandler(mouse_.event_handler());
    system::UnregisterSDLHandler(text_input_.event_handler());
}

void Manager::Update() {
    keyboard_.Update();
    mouse_.Update();
}

std::shared_ptr<Joystick> Manager::CreateJoystick(int device) {
    SDL_Joystick* sdljoystick = SDL_JoystickOpen(device);
    if (!sdljoystick) {
        debug::Log(debug::LogLevel::ERROR, "SDL_JoystickOpen failed for id ", device, " with error: ", SDL_GetError());
        return std::shared_ptr<Joystick>();
    }

    SDL_JoystickID id = SDL_JoystickInstanceID(sdljoystick);
    if (id == -1) {
        debug::Log(debug::LogLevel::CRITICAL, "SDL_JoystickInstanceID returned -1 for recently opened joystick");
        return std::shared_ptr<Joystick>();
    }

    auto& joystick = joysticks_[id];
    joystick.reset(new Joystick(sdljoystick));
    return joystick;
}

}  // namespace input
}  // namespace ugdk


