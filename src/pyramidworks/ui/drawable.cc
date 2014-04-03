
#include <pyramidworks/ui/drawable.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/opengl/vertexbuffer.h>
#include <ugdk/graphic/opengl/shaderuse.h>
#include <ugdk/structure/types.h>

#include <algorithm>

namespace pyramidworks {
namespace ui {

using ugdk::graphic::opengl::VertexBuffer;

Drawable::Drawable(const std::shared_ptr<ugdk::graphic::Primitive>& primitive)
    : primitive_(primitive) {}

Drawable::~Drawable() {}

void Drawable::Draw(ugdk::graphic::Canvas& canvas) const {

    if (draw_setup_function_)
        draw_setup_function_(this, canvas);

    canvas.PushAndCompose(-hotspot_);

    ugdk::graphic::opengl::ShaderUse shader(primitive_->shader_program());

    shader.SendTexture(0, primitive_->texture());
    shader.SendGeometry(canvas.current_geometry());
    shader.SendEffect(canvas.current_visualeffect());
    primitive_->drawfunction()(*primitive_, shader);

    canvas.PopGeometry();
}

const ugdk::math::Vector2D& Drawable::size() const {
    const_cast<Drawable*>(this)->updateSize();
    return size_;
}

void Drawable::updateSize() {
    auto& data = *primitive_->vertexdata();
    auto buffer = primitive_->vertexdata()->buffer().get();

    VertexBuffer::Bind bind(*buffer);
    VertexBuffer::Mapper mapper(*buffer);

    ugdk::uint8* ptr = static_cast<ugdk::uint8*>(mapper.get());

    GLfloat* v1 = reinterpret_cast<GLfloat*>(ptr);
    GLfloat min_x = v1[0];
    GLfloat max_x = v1[0];
    GLfloat min_y = v1[1];
    GLfloat max_y = v1[1];

    for (std::size_t i = 1; i < data.num_vertices(); ++i) {
        GLfloat* v = reinterpret_cast<GLfloat*>(ptr + i * data.vertex_size());
        min_x = std::min(min_x, v[0]);
        min_y = std::min(min_y, v[1]);
        max_x = std::max(max_x, v[0]);
        max_y = std::max(max_y, v[1]);
    }

    size_.x = max_x - min_x; 
    size_.y = max_y - min_y;
}

} // namespace pyramidworks
} // namespace ugdk

