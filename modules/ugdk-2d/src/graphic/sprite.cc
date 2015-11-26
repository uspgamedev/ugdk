#include <ugdk/graphic/sprite.h>

#include <ugdk/graphic/textureatlas.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/graphic/spriteanimationframe.h>

namespace ugdk {
namespace graphic {

PrimitiveControllerSprite::PrimitiveControllerSprite(const TextureAtlas *spritesheet)
: spritesheet_(spritesheet)
{}

PrimitiveControllerSprite::~PrimitiveControllerSprite() {}

void PrimitiveControllerSprite::ChangeToAnimationFrame(const SpriteAnimationFrame& frame) {
    auto&& piece = spritesheet_->PieceAt(frame.atlas_frame_name());

    owner_->set_visualeffect(frame.effect());
    VertexDataManipulation::SetUsingSpriteFrameInformation(*owner_->vertexdata(), position_, frame, piece);
}

void PrimitiveControllerSprite::ChangeToAtlasFrame(const std::string& frame_name) {
    auto&& piece = spritesheet_->PieceAt(frame_name);
    ChangeToBoundPiece(piece);
}

void PrimitiveControllerSprite::ChangeToAtlasFrame(std::size_t frame_number) {
    auto&& piece = spritesheet_->PieceAt(frame_number);
    ChangeToBoundPiece(piece);
}
    
void PrimitiveControllerSprite::ChangeToBoundPiece(TextureAtlas::BoundPiece& piece) {
    glm::vec4 top_left = glm::vec4(static_cast<float>(piece.offset().x), static_cast<float>(piece.offset().y), 0.0f, 1.0f);
    glm::vec4 bottom_right = glm::vec4(piece.trimmed_size().x + static_cast<float>(piece.offset().x),
                                       piece.trimmed_size().y + static_cast<float>(piece.offset().y),
                                       0.0, 1.0);
    VertexDataManipulation::SetToAbsoluteRectangleWithAtlasPiece(*owner_->vertexdata(), top_left, bottom_right, piece);
}
    
}  // namespace graphic
}  // namespace ugdk
