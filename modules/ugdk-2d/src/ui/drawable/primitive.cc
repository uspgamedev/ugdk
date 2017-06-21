
#include <ugdk/ui/drawable/primitive.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/canvas.h>
#include <ugdk/graphic/module.h>
#include <ugdk/graphic/textureunit.h>
#include <ugdk/structure/types.h>

#include <algorithm>
#include <numeric>

namespace ugdk {
namespace ui {

namespace {
    struct VertexXY {
        float x, y;
    };
}

DrawablePrimitive::DrawablePrimitive(const std::shared_ptr<ugdk::graphic::Primitive>& primitive)
    : primitive_(primitive) {}

DrawablePrimitive::~DrawablePrimitive() {}

void DrawablePrimitive::Draw(ugdk::graphic::Canvas& canvas) const {

    if (draw_setup_function_)
        draw_setup_function_(this, canvas);

    canvas.PushAndCompose(-hotspot_);
    canvas.ChangeShaderProgram(primitive_->shader_program());

    ugdk::graphic::TextureUnit unit = ugdk::graphic::manager().ReserveTextureUnit(primitive_->texture());
    canvas.SendUniform("drawable_texture", unit);
    primitive_->drawfunction()(*primitive_, canvas);

    canvas.PopGeometry();
}

const ugdk::math::Vector2D& DrawablePrimitive::size() const {
    const_cast<DrawablePrimitive*>(this)->UpdateSize();
    return size_;
}

void DrawablePrimitive::UpdateSize() {
    auto& data = *primitive_->vertexdata();
    ugdk::graphic::VertexData::Mapper mapper(data);

    float min_x = std::numeric_limits<float>::max(),
          max_x = std::numeric_limits<float>::min(),
          min_y = std::numeric_limits<float>::max(),
          max_y = std::numeric_limits<float>::min();

    for (std::size_t i = 0; i < data.num_vertices(); ++i) {
        VertexXY* p = mapper.Get<VertexXY>(i);
        min_x = std::min(min_x, p->x);
        min_y = std::min(min_y, p->y);
        max_x = std::max(max_x, p->x);
        max_y = std::max(max_y, p->y);
    }
    size_.x = max_x - min_x; 
    size_.y = max_y - min_y;
}

} // namespace ui
} // namespace ugdk

