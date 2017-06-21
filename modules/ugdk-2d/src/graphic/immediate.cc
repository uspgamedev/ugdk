#include <ugdk/graphic/immediate.h>

#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/graphic/canvas.h>
#include "ugdk/graphic/module.h"
#include "gltexture.h"

namespace ugdk {
namespace graphic {
namespace immediate {

namespace {

void RectangleVertexData(Canvas& canvas, VertexData& data, const GLTexture* texture) {
    auto sprite_unit = ugdk::graphic::manager().ReserveTextureUnit(texture);
    canvas.SendUniform("drawable_texture", sprite_unit);
    PrimitiveSetup::Rectangle::RenderVertexData(canvas, data);
}

}

void Rectangle(ugdk::graphic::Canvas& canvas, const math::Vector2D& offset, const GLTexture* texture) {
    Rectangle(canvas, offset, math::Vector2D(texture->width(), texture->height()), texture);
}

void Rectangle(ugdk::graphic::Canvas& canvas, const math::Vector2D& offset,
               const math::Vector2D& size, const GLTexture* texture) {

    VertexData data(PrimitiveSetup::Rectangle::vertexdata_specification);
    VertexDataManipulation::SetToRectangleAtOrigin(data, size);

    RectangleVertexData(canvas, data, texture);
}

void Rectangle(ugdk::graphic::Canvas& canvas, const math::Vector2D& offset,
               const TextureAtlas* atlas,
               const action::SpriteAnimationFrame& animation_frame) {
    auto&& piece = atlas->PieceAt(animation_frame.atlas_frame_name());

    VertexData data(PrimitiveSetup::Sprite::vertexdata_specification);
    VertexDataManipulation::SetUsingSpriteFrameInformation(data, offset, animation_frame, piece);
    RectangleVertexData(canvas, data, atlas->texture());
}

} // namespace immediate
} // namespace graphic
} // namespace ugdk
