#ifndef UGDK_GRAPHIC_PRIMITIVESETUP_H_
#define UGDK_GRAPHIC_PRIMITIVESETUP_H_

#include <ugdk/graphic.h>
#include <ugdk/internal.h>
#include <ugdk/action.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitivecontroller.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/system/compatibility.h>

#include <string>
#include <utility>

namespace ugdk {
namespace graphic {

namespace VertexDataManipulation {

void ApplyPositionOffset(VertexData& data, const math::Vector2D& offset);
void SetToRectangleAtOrigin(VertexData& data, const math::Vector2D& size);
void SetToAbsoluteRectangleWithAtlasPiece(VertexData& data, const glm::vec4& top_left, const glm::vec4& bottom_right, const TextureAtlas::BoundPiece& piece);
void SetUsingSpriteFrameInformation(VertexData& data, const math::Vector2D& position, const action::SpriteAnimationFrame&, const TextureAtlas::BoundPiece&);

} // namespace VertexDataManipulation

namespace PrimitiveSetup {

struct Sprite {
    static const VertexDataSpecification vertexdata_specification;

    static void Prepare(Primitive&, const TextureAtlas* spritesheet);
    static void Render(const Primitive&, opengl::ShaderUse& shader_use);

    static std::shared_ptr<action::SpriteAnimationPlayer> CreateSpriteAnimationPlayer(Primitive&, const action::SpriteAnimationTable* table);
};

struct Rectangle {
    static const VertexDataSpecification vertexdata_specification;

    static void Prepare(Primitive&, const internal::GLTexture*, const math::Vector2D& size);
    static void Prepare(Primitive&, const internal::GLTexture*);
    static void Render(const Primitive&, opengl::ShaderUse& shader_use);
};

} // namespace PrimitiveSetup

} // namespace graphic
} // namespace ugdk

#endif // UGDK_GRAPHIC_PRIMITIVESETUP_H_
