
#include <ugdk/debug/log.h>
#include <ugdk/desktop/manager.h>
#include <ugdk/desktop/window.h>
#include <ugdk/system/engine.h>

#include <system/sdleventhandler.h>
#include <cassert>

namespace ugdk {
namespace desktop {

using std::weak_ptr;
using std::shared_ptr;

class DesktopSDLEventHandler : public system::SDLEventHandler {
public:
    DesktopSDLEventHandler(Manager& manager) : manager_(manager) {}

    std::unordered_set<Uint32> TypesHandled() const override {
        return { SDL_WINDOWEVENT };
    }

    void Handle(const ::SDL_Event& sdlevent) const override {
        switch (sdlevent.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                // no-op
                break;

            case SDL_WINDOWEVENT_CLOSE:
                uint32_t i; 
                for (i = 0; i < manager_.windows_.size(); i++)
                    if (manager_.windows_[i]->id() == sdlevent.window.windowID)
                        break;
                manager_.DestroyWindow(i);
                break;

            default:
                break; // ignore
        }
    }

private:
    void operator=(const DesktopSDLEventHandler&);
    Manager& manager_;
};

Manager::Manager() {}
Manager::~Manager() {}

bool Manager::Initialize() {
    sdlevent_handler_.reset(new DesktopSDLEventHandler(*this));
    system::RegisterSDLHandler(sdlevent_handler_.get());
    return true;
}

void Manager::Release() {
    system::UnregisterSDLHandler(sdlevent_handler_.get());
    sdlevent_handler_.reset();
    windows_.clear();
}

weak_ptr<Window> Manager::CreateWindow(const WindowSettings& settings) {
    Uint32 flags = SDL_WINDOW_OPENGL;
    if(settings.fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    SDL_Window* window  = SDL_CreateWindow(settings.title.c_str(),
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                           settings.size.x, settings.size.y,
                                           flags);
    if(!window) {
       // Couldn't create the window.
       debug::Log(debug::LogLevel::ERROR, "UGDK",
                  "desktop::Manager - Failed to create the window: ", SDL_GetError());
        return std::shared_ptr<desktop::Window>();
    }

    return RegisterAndGetWindow(std::shared_ptr<desktop::Window>(new desktop::Window(window)));
}

void Manager::DestroyWindow(uint32_t index) {
    if (0 <= index && index < windows_.size());
        windows_.erase(windows_.begin() +index);
        
    auto &map = map_id_to_window_;
    std::shared_ptr<Window> target_window = window(index).lock();
    if (target_window)
        map.erase(SDL_GetWindowID(target_window->sdl_window_));
    else {
        std::vector<uint32_t> kill_key_vector;
        for (auto &pair : map)
            if (!pair.second.lock())
                kill_key_vector.push_back(pair.first);
        for (auto key : kill_key_vector)
            map.erase(key);
    }
}

std::weak_ptr<Window> Manager::RegisterAndGetWindow(const shared_ptr<Window>& new_window) {
    assert(new_window); //Can't register non-existent window.

    windows_.push_back(new_window);
    map_id_to_window_[SDL_GetWindowID( (new_window->sdl_window_) )] = new_window;

    if(!primary_window_.lock()) //If we don't have a prim. win. then set it
        primary_window_ = new_window;
   
    return new_window;
}

std::weak_ptr<Window> Manager::window(uint32 index) const {
    if (index < windows_.size()) {
        return windows_[index];
    }
    else {
        return weak_ptr<Window>();
    }
}
std::weak_ptr<Window> Manager::WindowById(uint32_t id) {
    return map_id_to_window_[id];
}

uint32_t Manager::num_windows() {
    return windows_.size();
}
void Manager::PresentAll() {
    for(const auto& window : windows_)
        window->Present();
}

}  // namespace desktop
}  // namespace ugdk
