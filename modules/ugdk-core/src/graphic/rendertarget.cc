#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/module.h>

#include <ugdk/graphic/exceptions.h>
#include <ugdk/graphic/opengl.h>

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

bool RenderTarget::IsValid() const {
    return is_bound_;
}

const math::Geometry& RenderTarget::projection_matrix() const {
    return projection_matrix_;
}

void RenderTarget::set_projection_matrix(const math::Geometry& geometry) {
    projection_matrix_ = geometry;
}

void RenderTarget::Render() {
    my_renderer_.AssignTo(this);
    my_renderer_.Process();
    my_renderer_.Present();
}

void RenderTarget::Clear(structure::Color color) {
    system::AssertCondition<system::InvalidOperation>(IsActive(), "RenderTarget must be active for Clear.");
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    internal::AssertNoOpenGLError();
}

void RenderTarget::Bind() {
    system::AssertCondition<system::InvalidOperation>(!IsActive(), "RenderTarget must be inactive for Bind.");
    is_bound_ = true;
    UpdateViewport();
}

void RenderTarget::Unbind() {
    is_bound_ = false;
}

Renderer* RenderTarget::MyRenderer() {
    return &my_renderer_; 
}

}  // namespace graphic
}  // namespace ugdk
