#include <ugdk/graphic/sprite.h>

#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/action/spriteanimationframe.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

PrimitiveControllerSprite::PrimitiveControllerSprite(const TextureAtlas *spritesheet)
: spritesheet_(spritesheet)
{}

PrimitiveControllerSprite::~PrimitiveControllerSprite() {}

void PrimitiveControllerSprite::ChangeToAnimationFrame(const action::SpriteAnimationFrame& frame) {
    auto&& piece = spritesheet_->PieceAt(frame.atlas_frame_name());

    owner_->set_visualeffect(frame.effect());
    VertexDataManipulation::SetUsingSpriteFrameInformation(*owner_->vertexdata(), position_, frame, piece);
}

void PrimitiveControllerSprite::ChangeToAtlasFrame(const std::string& frame_name) {
    auto&& piece = spritesheet_->PieceAt(frame_name);

    glm::vec4 top_left = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 bottom_right = glm::vec4(piece.size().x, piece.size().y, 0.0, 1.0);
    VertexDataManipulation::SetToAbsoluteRectangleWithAtlasPiece(*owner_->vertexdata(), top_left, bottom_right, piece);
}
    
}  // namespace graphic
}  // namespace ugdk
