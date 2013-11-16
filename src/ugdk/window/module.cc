#include <ugdk/window/module.h>

#include "SDL.h"

namespace ugdk {
namespace window {

namespace {

bool errlog(const std::string& msg) {
    fprintf(stderr, "ugdk::window::Manager Error Log - %s\n", msg.c_str());
    return false;
}

static Manager* reference_ = nullptr;

}

bool Initialize(Manager* manager) {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        return errlog("Failed to initialize SDL_VIDEO: " + std::string(SDL_GetError()));

    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if(manager && manager->Initialize()) {
        // The manager initialized correctly, so we can use it.
        reference_ = manager;
        return true;
    } else {
        // Error initializing the manager, delete it and don't activate the module.
        delete manager;
        // TODO: log the error.
        return false;
    }
}

void Release() {
    if(reference_)
        reference_->Release();
    delete reference_;
    reference_ = nullptr;
}

Manager* manager() {
    return reference_;
}

} // namespace window
} // namespace ugdk
