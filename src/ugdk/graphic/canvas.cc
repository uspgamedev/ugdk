#include <ugdk/graphic/canvas.h>

#include <ugdk/graphic/rendertarget.h>

#include <cmath>
#include <cassert>

namespace ugdk {
namespace graphic {

Canvas::Canvas(RenderTarget* render_target)
    : render_target_(render_target)
{
    geometry_stack_.emplace_back(render_target_->projection_matrix());
    visualeffect_stack_.emplace_back();

    render_target->Bind();
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

void Canvas::Clear(Color color) {
    render_target_->Clear(color);
}
    
}  // namespace graphic
}  // namespace ugdk
