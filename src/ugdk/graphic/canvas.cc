#include <ugdk/graphic/canvas.h>

#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/opengl/shaderprogram.h>

#include <cmath>
#include <cassert>

namespace ugdk {
namespace graphic {

namespace {
Canvas* ACTIVE_CANVAS = nullptr;
}

Canvas::Canvas(RenderTarget* render_target)
    : render_target_(render_target)
    , previous_canvas_(nullptr)
    , next_canvas_(nullptr)
{
    geometry_stack_.emplace_back(render_target_->projection_matrix());
    visualeffect_stack_.emplace_back();

    if (ACTIVE_CANVAS) {
        previous_canvas_ = ACTIVE_CANVAS;
        previous_canvas_->Unbind();
        previous_canvas_->next_canvas_ = this;
    }
    ACTIVE_CANVAS = this;
    Bind();
}

Canvas::~Canvas() {
    if (IsActive()) {
        Unbind();
        if (next_canvas_)
            ACTIVE_CANVAS = next_canvas_;
        else
            ACTIVE_CANVAS = previous_canvas_;

        if (ACTIVE_CANVAS)
            ACTIVE_CANVAS->Bind();
    }

    if (next_canvas_)
        next_canvas_->previous_canvas_ = previous_canvas_;
    if (previous_canvas_)
        previous_canvas_->next_canvas_ = next_canvas_;
}

void Canvas::ChangeShaderProgram(const opengl::ShaderProgram* shader_program) {
    shader_program_ = shader_program;
    if (IsActive()) {
        glUseProgram(shader_program_->id());
    }
}

bool Canvas::IsActive() const {
    return ACTIVE_CANVAS == this;
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

void Canvas::Bind() {
    render_target_->Bind();
    if (shader_program_)
        glUseProgram(shader_program_->id());
}

void Canvas::Unbind() {
    render_target_->Unbind();
    glUseProgram(0);
}

}  // namespace graphic
}  // namespace ugdk
