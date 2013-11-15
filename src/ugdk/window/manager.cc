#include <ugdk/window/manager.h>

#include <cmath>
#include <cassert>

#include "GL/glew.h"
#ifdef _WIN32
#   include "windows.h"
#   include "GL/wglew.h"
#endif
#define NO_SDL_GLEXT
#include "SDL.h"
#include "SDL_image.h"

#include <ugdk/action/scene.h>

namespace ugdk {
namespace window {

namespace {
bool errlog(const std::string& msg) {
    fprintf(stderr, "ugdk::window::Manager Error Log - %s\n", msg.c_str());
    return false;
}
}

static ugdk::math::Vector2D default_resolution(800.0, 600.0);
    
WindowSettings::WindowSettings()
    : fullscreen(false), vsync(true) {}

Manager::Manager(const WindowSettings& settings) 
    :   settings_(settings)
    ,   window_(nullptr)
{}

bool Manager::Initialize() {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
        return errlog("Failed to initialize SDL_VIDEO: " + std::string(SDL_GetError()));
    
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window_ = SDL_CreateWindow(settings_.window_title.c_str(),
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               settings_.resolution.x, settings_.resolution.y,
                               SDL_WINDOW_OPENGL);

    if(!window_) {
        // Couldn't create the window.
        // TODO: Log the error
        return errlog("Failed to create the window: " + std::string(SDL_GetError()));
    }

    if(settings_.window_icon.length() > 0) {
        SDL_Surface* icon = IMG_Load(settings_.window_icon.c_str());
        SDL_SetWindowIcon(window_, icon);
        SDL_FreeSurface(icon);
    }
    return true;
}

void Manager::Release() {
    SDL_DestroyWindow(window_);
}

void Manager::Present() {
    // Swap the buffers to show the backbuffer to the user.
    SDL_GL_SwapWindow(window_);
}

bool Manager::ChangeSettings(const WindowSettings& new_settings) {
    settings_ = new_settings;

    // SDL_SetWindowSize fails silently when the window is fullscreen.
    // So, first remove fullscreen.
    SDL_SetWindowFullscreen(window_, 0);

    // Now resize the window.
    SDL_SetWindowSize(window_, new_settings.resolution.x, new_settings.resolution.y);

    // And set it to fullscreen again, if needed.
    // This may leave the window in a broken state if the display doesn't support
    // the requested resolution, but SDL_SetWindowFullscreen reports no errors in such cases.
    if(new_settings.fullscreen)
        SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN);

    SDL_GL_SetSwapInterval(new_settings.vsync ? 1 : 0);
    return true;
}

}  // namespace window
}  // namespace ugdk
