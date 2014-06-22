#include <ugdk/graphic/framebuffer.h>

#include <ugdk/internal/opengl.h>
#include <ugdk/internal/gltexture.h>
#include <ugdk/math/integer2D.h>

#include "SDL_video.h"

#include <cmath>
#include <cassert>

namespace ugdk {
namespace graphic {

namespace {
    internal::GLTexture* CreateTexture(const math::Integer2D& size) {
        internal::GLTexture* texture = ugdk::internal::GLTexture::CreateRawTexture(size.x, size.y);
        glBindTexture(GL_TEXTURE_2D, texture->id());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width(), texture->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }
}

Framebuffer::Framebuffer(const math::Integer2D& size)
: texture_(CreateTexture(size))
, is_bound_(false)
{                           
    glGenFramebuffers(1, &gl_buffer_);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &gl_buffer_);
}

std::shared_ptr<Framebuffer> Framebuffer::Create(const math::Integer2D& size) {
    std::shared_ptr<Framebuffer> framebuffer(new Framebuffer(size));
    internal::AssertNoOpenGLError();
    
    return framebuffer;
}

void Framebuffer::Resize(const math::Vector2D& size) {
    assert(!is_bound_);
    assert(false);
}

void Framebuffer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
    internal::AssertNoOpenGLError();
}

void Framebuffer::Clear(Color color) {
    glClearColor(color.r, color.g, color.b, color.a);
    Clear();
}
    
void Framebuffer::Bind() {
    is_bound_ = true;
    glBindFramebuffer(GL_FRAMEBUFFER, gl_buffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_->id(), 0);
}

void Framebuffer::Unbind() {
    is_bound_ = false;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}  // namespace graphic
}  // namespace ugdk
