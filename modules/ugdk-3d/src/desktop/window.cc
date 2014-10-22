#include <ugdk/desktop/3D/window.h>

#include <ugdk/debug/profiler.h>

#include "OgreRenderWindow.h"

namespace ugdk {
namespace desktop {
namespace mode3d {

Window::Window(Ogre::RenderWindow* ogre_window)
    : ogre_window_(ogre_window)
{}

void Window::Present() {
    ogre_window_->update();
}

void Window::ChangeSettings(const math::Integer2D& size, bool fullscreen, bool vsync) {
    ogre_window_->setFullscreen(fullscreen, size.x, size.y);
    ogre_window_->setVSyncEnabled(vsync);
}


uint32 Window::id() const {
    return 1; //SDL_GetWindowID(sdl_window_); // FIXME
}
    
const char* Window::title() const {
    return ogre_window_->getName().c_str();
}

math::Integer2D Window::size() const {
    return math::Integer2D( ogre_window_->getWidth(), ogre_window_->getHeight() );
}

bool Window::fullscreen() const {
    return ogre_window_->isFullScreen();
}

bool Window::vsync() const {
    return ogre_window_->isVSyncEnabled();
}

}  // namespace mode3d
}  // namespace desktop
}  // namespace ugdk
