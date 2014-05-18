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

void PrimitiveControllerSprite::ChangeToFrame(const action::SpriteAnimationFrame& frame) {
    const auto& spritesheet_frame = spritesheet_->PieceAt(frame.spritesheet_frame());

    owner_->set_visualeffect(frame.effect());
    VertexDataManipulation::SetUsingSpriteFrameInformation(*owner_->vertexdata(), position_, frame, spritesheet_frame);
}
    
}  // namespace graphic
}  // namespace ugdk
