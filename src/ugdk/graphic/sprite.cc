#include <ugdk/graphic/sprite.h>

#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/primitivesetup.h>
#include <ugdk/action/spriteanimationframe.h>


namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

PrimitiveControllerSprite::PrimitiveControllerSprite(const Spritesheet *spritesheet)
: spritesheet_(spritesheet)
, owner_(nullptr)
{}

PrimitiveControllerSprite::~PrimitiveControllerSprite() {}

void PrimitiveControllerSprite::set_owner(Primitive* owner) {
    owner_ = owner;
}

void PrimitiveControllerSprite::ChangeToFrame(const action::SpriteAnimationFrame& frame) {
    const auto& spritesheet_frame = spritesheet_->frame(frame.spritesheet_frame());

    owner_->set_visualeffect(frame.effect());
    VertexDataManipulation::SetUsingSpriteFrameInformation(*owner_->vertexdata(), position_, frame, spritesheet_frame);
}
    
void PrimitiveControllerSprite::ChangePosition(const math::Vector2D& position) {
    VertexDataManipulation::ApplyPositionOffset(*owner_->vertexdata(), position - position_);
    position_ = position;
}

}  // namespace graphic
}  // namespace ugdk
