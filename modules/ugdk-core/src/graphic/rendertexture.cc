#include <ugdk/graphic/rendertexture.h>

#include <ugdk/graphic/opengl.h>
#include <ugdk/math/integer2D.h>
#include "gltexture.h"

#include "SDL_video.h"

#include <cmath>
#include <cassert>

namespace ugdk {
namespace graphic {

namespace {
    graphic::GLTexture* CreateTexture(const math::Integer2D& size) {
        graphic::GLTexture* texture = ugdk::graphic::GLTexture::CreateRawTexture(size.x, size.y);
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

RenderTexture::RenderTexture(std::unique_ptr<graphic::GLTexture>&& texture)
    : texture_(std::move(texture))
{
    glGenFramebuffers(1, &gl_buffer_);
}

RenderTexture::RenderTexture(const math::Integer2D& size)
    : texture_(CreateTexture(size))
{                           
    glGenFramebuffers(1, &gl_buffer_);
}

RenderTexture::~RenderTexture() {
    glDeleteFramebuffers(1, &gl_buffer_);
}

math::Vector2D RenderTexture::size() const {
    return math::Vector2D(texture_->width(), texture_->height());
}

void RenderTexture::Resize(const math::Vector2D& size) {
    projection_matrix_ = math::Geometry(math::Vector2D(-1.0, 1.0), math::Vector2D(2.0/size.x, -2.0/size.y));
}

void RenderTexture::Use() {
    /* WE NEED TO IMPLEMENT SOMETHING REASONABLE HERE */
}

void RenderTexture::Bind() {
    RenderTarget::Bind();
    glBindFramebuffer(GL_FRAMEBUFFER, gl_buffer_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_->id(), 0);
}

void RenderTexture::Unbind() {
    RenderTarget::Unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::UpdateViewport() {
    glViewport(0, 0, texture_->width(), texture_->height());
}

}  // namespace graphic
}  // namespace ugdk
