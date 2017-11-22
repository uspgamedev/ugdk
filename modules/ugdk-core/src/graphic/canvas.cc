
#include <ugdk/graphic/canvas.h>

#include <ugdk/graphic/exceptions.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/rendertarget.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/shaderprogram.h>
#include "vertexbuffer.h"
#include "gltexture.h"

#include <cassert>

namespace ugdk {
namespace graphic {

namespace {
Canvas* ACTIVE_CANVAS = nullptr;
}

Canvas::Canvas(RenderTarget* render_target)
    : render_target_(render_target)
    , shader_program_(nullptr)
    , previous_canvas_(nullptr)
    , next_canvas_(nullptr)
{
    geometry_stack_.emplace_back(render_target_->projection_matrix());
    visualeffect_stack_.emplace_back();
}

Canvas::~Canvas() {
    if (this->IsValid())
        Unbind();
}

void Canvas::ChangeShaderProgram(const ShaderProgram* shader_program) {
    shader_program_ = shader_program;
    if (shader_program_) {
        glUseProgram(shader_program_->id());
        SendGeometry();
        SendEffect();
    } else {
        glUseProgram(0);
    }
}

bool Canvas::IsActive() const {
    return ACTIVE_CANVAS == this;
}

bool Canvas::IsValid() const {
    return render_target_->IsValid(); 
}

math::Vector2D Canvas::size() const {
    return render_target_->size();
}

void Canvas::PushAndCompose(const math::Geometry& geometry) {
    geometry_stack_.reserve(geometry_stack_.size() + 1);
    geometry_stack_.emplace_back(geometry_stack_.back());
    geometry_stack_.back().Compose(geometry);
    if (shader_program_)
        SendGeometry();
}

void Canvas::PushAndCompose(const structure::VisualEffect& effect) {
    visualeffect_stack_.reserve(visualeffect_stack_.size() + 1);
    visualeffect_stack_.emplace_back(visualeffect_stack_.back());
    visualeffect_stack_.back().Compose(effect);
    if (shader_program_)
        SendEffect();
}

void Canvas::PopGeometry() {
    system::AssertCondition<system::InvalidOperation>(geometry_stack_.size() > 1, "Can only pop a Geometry after pushing at least one.");
    geometry_stack_.pop_back();
    if (shader_program_)
        SendGeometry();
}

void Canvas::PopVisualEffect() {
    system::AssertCondition<system::InvalidOperation>(visualeffect_stack_.size() > 1, "Can only pop a VisualEffect after pushing at least one.");
    visualeffect_stack_.pop_back();
    if (shader_program_)
        SendEffect();
}

void Canvas::Clear(structure::Color color) {
    render_target_->Clear(color);
}

void Canvas::SendUniform(const std::string& name, float t1) {
    glUniform1f(shader_program_->UniformLocation(name), t1);
    internal::AssertNoOpenGLError();
}
void Canvas::SendUniform(const std::string& name, float t1, float t2) {
    glUniform2f(shader_program_->UniformLocation(name), t1, t2);
    internal::AssertNoOpenGLError();
}
void Canvas::SendUniform(const std::string& name, const math::Vector2D& v) {
    SendUniform(name, static_cast<float>(v.x), static_cast<float>(v.y));
}
void Canvas::SendUniform(const std::string& name, float t1, float t2, float t3) {
    glUniform3f(shader_program_->UniformLocation(name), t1, t2, t3);
    internal::AssertNoOpenGLError();
}
void Canvas::SendUniform(const std::string& name, float t1, float t2, float t3, float t4) {
    glUniform4f(shader_program_->UniformLocation(name), t1, t2, t3, t4);
    internal::AssertNoOpenGLError();
}

void Canvas::SendUniform(const std::string& name, const TextureUnit& unit) {
    glUniform1i(shader_program_->UniformLocation(name), unit.id());
}

void Canvas::Bind() {
    if (shader_program_)
        glUseProgram(shader_program_->id());
}

void Canvas::Unbind() {
    glUseProgram(0);
}

void Canvas::SendGeometry() {
    glUniformMatrix4fv(shader_program_->UniformLocation("geometry_matrix"), 1, GL_FALSE, &current_geometry().AsMat4()[0][0]);
    internal::AssertNoOpenGLError();
}
     
void Canvas::SendEffect() {
    const structure::Color& c = current_visualeffect().color();
    glUniform4f(shader_program_->UniformLocation("effect_color"),
                static_cast<GLfloat>(c.r),
                static_cast<GLfloat>(c.g),
                static_cast<GLfloat>(c.b),
                static_cast<GLfloat>(c.a));
    internal::AssertNoOpenGLError();
}

void Canvas::SendVertexData(const VertexData& data, VertexType type, size_t offset, int size,
                            int vertex_per_data) {
    VertexBuffer::Bind bind(*data.buffer().get());

    unsigned int location = manager().LocationForVertexType(type);
    glEnableVertexAttribArray(location);

    glVertexAttribPointer(
        location,            // location
        size,                // size
        GL_FLOAT,            // data type
        GL_FALSE,            // normalized?
        static_cast<int>(data.vertex_size())/vertex_per_data, // ammount of bytes to offset for each element. 0 means size*sizeof(type)
        data.buffer()->getPointer(offset) // array buffer offset
    );
    internal::AssertNoOpenGLError();
}

void Canvas::DrawArrays(DrawMode mode, int first_vertex, int vertex_count) {
    glDrawArrays(mode.mode(), first_vertex, vertex_count);
}

}  // namespace graphic
}  // namespace ugdk
