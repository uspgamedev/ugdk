#include <ugdk/input/mouse.h>

#include <ugdk/action/scene.h>
#include <ugdk/input/events.h>
#include <ugdk/system/engine.h>

#include <system/sdleventhandler.h>

namespace ugdk {
namespace input {

class MouseInputSDLEventHandler : public system::SDLEventHandler {
  public:
    MouseInputSDLEventHandler(Mouse& mouse) : mouse_(mouse) {}

    static system::EventHandler& handler() {
        return system::CurrentScene().event_handler();
    }

    std::unordered_set<Uint32> TypesHandled() const override {
        return { SDL_MOUSEMOTION, SDL_MOUSEBUTTONDOWN,
                 SDL_MOUSEBUTTONUP, SDL_MOUSEWHEEL };
    }

    void Handle(const ::SDL_Event& sdlevent) const override {
        if(sdlevent.type == SDL_MOUSEMOTION)
            MouseMotionHandler(sdlevent);
        else if(sdlevent.type == SDL_MOUSEBUTTONDOWN)
            MouseButtonDownHandler(sdlevent);
        else if(sdlevent.type == SDL_MOUSEBUTTONUP)
            MouseButtonUpHandler(sdlevent);
        else if(sdlevent.type == SDL_MOUSEWHEEL)
            MouseWheelHandler(sdlevent);
        else
            assert(false);
    }

    void MouseMotionHandler(const ::SDL_Event& sdlevent) const {
        mouse_.position_.x = sdlevent.motion.x;
        mouse_.position_.y = sdlevent.motion.y;
        handler().RaiseEvent(MouseMotionEvent(
            mouse_.position_,
            math::Integer2D(sdlevent.motion.xrel, sdlevent.motion.yrel)
        ));
    }

    void MouseButtonDownHandler(const ::SDL_Event& sdlevent) const {
        mouse_.position_.x = sdlevent.button.x;
        mouse_.position_.y = sdlevent.button.y;
        MouseButton button = static_cast<MouseButton>(sdlevent.button.button-1);
        handler().RaiseEvent(MouseButtonPressedEvent(mouse_.position_, button));
        mouse_.state_.insert(button);
    }

    void MouseButtonUpHandler(const ::SDL_Event& sdlevent) const {
        mouse_.position_.x = sdlevent.button.x;
        mouse_.position_.y = sdlevent.button.y;
        MouseButton button = static_cast<MouseButton>(sdlevent.button.button-1);
        handler().RaiseEvent(MouseButtonReleasedEvent(mouse_.position_, button));
        mouse_.state_.erase(button);
    }

    void MouseWheelHandler(const ::SDL_Event& sdlevent) const {
        system::CurrentScene().event_handler().RaiseEvent(MouseWheelEvent(
            math::Integer2D(sdlevent.wheel.x, sdlevent.wheel.y)
        ));
    }


  private:
    void operator=(const MouseInputSDLEventHandler&);

    Mouse& mouse_;
};

bool Mouse::IsDown(const MouseButton& button) const {
    return state_.count(button) > 0;
}
bool Mouse::IsUp(const MouseButton& button) const {
    return state_.count(button) == 0;
}

bool Mouse::IsPressed(const MouseButton& button) const {
    return IsDown(button) && state_previous_.count(button) == 0;
}
bool Mouse::IsReleased(const MouseButton& button) const {
    return IsUp(button) && state_previous_.count(button) > 0;
}

Mouse::Mouse() : event_handler_(new MouseInputSDLEventHandler(*this)) {}

Mouse::~Mouse() {}

void Mouse::Update() {
    state_previous_ = state_;
}

const system::SDLEventHandler* Mouse::event_handler() {
    return event_handler_.get();
}

} // namespace input
} // namespace ugdk
