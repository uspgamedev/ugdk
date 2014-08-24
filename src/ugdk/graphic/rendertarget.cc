#include <ugdk/graphic/rendertarget.h>

#include <ugdk/internal/opengl.h>

namespace ugdk {
namespace graphic {

RenderTarget::RenderTarget()
    : is_bound_(false)
{
}

RenderTarget::~RenderTarget() {
    if(IsActive())
        Unbind();
}

bool RenderTarget::IsActive() const {
    return is_bound_;
}

const Geometry& RenderTarget::projection_matrix() const {
    return projection_matrix_;
}

void RenderTarget::Clear(Color color) {
    // TODO: check if IsActive
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    internal::AssertNoOpenGLError();
}

void RenderTarget::Bind() {
    // TODO: check if !IsActive
    is_bound_ = true;
    UpdateViewport();
}

void RenderTarget::Unbind() {
    // TODO: check if IsActive
    is_bound_ = false;
}

}  // namespace graphic
}  // namespace ugdk
