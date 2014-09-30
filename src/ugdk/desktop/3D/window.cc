#include <ugdk/desktop/3D/window.h>


#include <ugdk/graphic/canvas.h>
#include <ugdk/debug/profiler.h>

namespace ugdk {
namespace desktop {
namespace mode3d {

Window::Window(Ogre::RenderWindow* ogre_window)
    : ogre_window_(ogre_window)
{}
    
Window::~Window() {
    //Do not believe this is needed, since Root destroys everything 
    ogre_window_->destroy();
    ogre_window_ = nullptr;
}

void Window::Present() {
    ogre_window_->update();
}

void Window::ChangeSettings(const math::Integer2D& size, bool fullscreen, bool vsync) {
    ogre_window_->setFullScreen(fullscreen, size.x, size.y);
    ogre_window_->setVSyncEnabled(vsync);
}


uint32 Window::id() const {
    return SDL_GetWindowID(sdl_window_);
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

bool vsync() const {
    return ogre_window_->isVSyncEnabled();
}

}  // namespace mode3d
}  // namespace desktop
}  // namespace ugdk
