
#include <ugdk/graphic/renderscreen.h>
#include <ugdk/desktop/window.h>
#include <ugdk/math/integer2D.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/system/exceptions.h>

#include "gltexture.h"
#include "SDL_video.h"

namespace ugdk {
namespace graphic {

RenderScreen::RenderScreen() : window_(std::weak_ptr<desktop::Window>()) {}

RenderScreen::RenderScreen(const std::weak_ptr<desktop::Window>& window) : window_(window) {}

void RenderScreen::SaveToTexture(graphic::GLTexture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->id());
    //glReadBuffer(GL_BACK); FIXME
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texture->width(), texture->height());
    internal::AssertNoOpenGLError();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void RenderScreen::AttachTo(const std::weak_ptr<desktop::Window>& weak_window) {
    if (weak_window.lock())
        window_ = weak_window;
}

bool RenderScreen::IsValid() const {
    return (window_.lock()!=std::shared_ptr<desktop::Window>());
}

void RenderScreen::UpdateViewport()  {
    if(auto window = window_.lock())
        glViewport(0, 0, window->size().x, window->size().y);
}
std::weak_ptr<desktop::Window> RenderScreen::Window() const {
    return window_;
}
void RenderScreen::Resize(const math::Vector2D &canvas_size) {
    size_ = canvas_size;
    projection_matrix_ = math::Geometry(math::Vector2D(-1.0, 1.0), math::Vector2D(2.0/size_.x, -2.0/size_.y));

}

math::Vector2D RenderScreen::size() const {
    return size_;
}


void RenderScreen::Use() {
    auto window = window_.lock();
    system::AssertCondition<system::InvalidOperation>(window.get(), "Can only Use non null windows");
    auto sdl_window = window->sdl_window_;
    system::AssertCondition<system::InvalidOperation>(sdl_window, "Can only Use non null sdl-windows");

    SDL_GLContext context = SDL_GL_GetCurrentContext();
    SDL_GL_MakeCurrent(sdl_window, context);
}

}
}