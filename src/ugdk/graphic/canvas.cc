#include <ugdk/graphic/canvas.h>

#include <cmath>
#include <cassert>

#include "GL/glew.h"
#ifdef _WIN32
#   include "windows.h"
#   include "GL/wglew.h"
#endif
#define NO_SDL_GLEXT
#include "SDL_video.h"

#include <ugdk/graphic/texture.h>
#include <ugdk/desktop/window.h>

namespace ugdk {
namespace graphic {

Canvas::Canvas(SDL_GLContext context) 
    : context_(context) {
        visualeffect_stack_.emplace_back();
}

Canvas::~Canvas() {
    SDL_GL_DeleteContext(context_);
}

std::shared_ptr<Canvas> Canvas::Create(const std::weak_ptr<desktop::Window>& window_weak, const math::Vector2D& size) {
    auto window = window_weak.lock();
    if(!window)
        return std::shared_ptr<Canvas>();

    SDL_GLContext context = SDL_GL_CreateContext(window->sdl_window_);
    if(!context)
        return std::shared_ptr<Canvas>(); //errlog("OpenGL context creation failed: " + string(SDL_GetError()));
    
    std::shared_ptr<Canvas> canvas(new Canvas(context));
    
    GLenum err = glewInit();
    if (GLEW_OK != err)
        return nullptr; //errlog("GLEW Error: " + string((const char*)(glewGetErrorString(err))));
    
    canvas->AttachTo(window_weak);
    canvas->Resize(size);
       
    // This hint can improve the speed of texturing when perspective-correct texture coordinate interpolation isn't needed, such as when using a glOrtho() projection.
    // From http://www.mesa3d.org/brianp/sig97/perfopt.htm
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    SDL_GL_SetSwapInterval(true ? 1 : 0);

    return canvas;
}

void Canvas::Resize(const math::Vector2D& size) {
    assert(geometry_stack_.size() <= 1);

    size_ = size;
    geometry_stack_.clear();

    // We want the following properties to our display:
    //   (0;0) is the top-left corner of the screen
    //   (w;h) is the bottom-right corner of the screen
    // Since by default, (0;0) is the center of the screen, with (-1;1) being 
    // the top-left and (1;-1) the bottom-right, we must do the following:
    //   - Offset it by (-1,1), correcting the origin
    //   - Invert the y-axis, so it grows in the direction we expect
    //   - Scale down by the (2/w;2/h), so it goes up to what we expect.
    geometry_stack_.emplace_back(math::Vector2D(-1.0, 1.0), math::Vector2D(2.0/size_.x, -2.0/size_.y));
}
    
void Canvas::AttachTo(const std::weak_ptr<desktop::Window>& window_weak) {
    auto window = window_weak.lock();
    if(window) {
        if(auto previous_window = attached_window_.lock())
            previous_window->attached_canvas_.reset();

        attached_window_ = window_weak;
        window->attached_canvas_ = this->shared_from_this();

        SDL_GL_MakeCurrent(window->sdl_window_, context_);
        UpdateViewport();
    }
}
    
void Canvas::UpdateViewport() {
    auto window = attached_window_.lock();
    if(window)
        glViewport(0, 0, window->size().x, window->size().y);
}
void Canvas::SaveToTexture(Texture* texture) {
    glBindTexture(GL_TEXTURE_2D, texture->gltexture());
    glReadBuffer(GL_BACK);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, texture->width(), texture->height());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Canvas::PushAndCompose(const Geometry& geometry) {
    geometry_stack_.reserve(geometry_stack_.size() + 1);
    geometry_stack_.emplace_back(geometry_stack_.back());
    geometry_stack_.back().Compose(geometry);
}

void Canvas::PushAndCompose(const VisualEffect& effect) {
    visualeffect_stack_.reserve(visualeffect_stack_.size() + 1);
    visualeffect_stack_.emplace_back(visualeffect_stack_.back());
    visualeffect_stack_.back().Compose(effect);
}

void Canvas::PopGeometry() {
    assert(geometry_stack_.size() > 1);
    geometry_stack_.pop_back();
}

void Canvas::PopVisualEffect() {
    assert(visualeffect_stack_.size() > 1);
    visualeffect_stack_.pop_back();
}

}  // namespace graphic
}  // namespace ugdk
