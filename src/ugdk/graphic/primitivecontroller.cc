#include <ugdk/graphic/primitivecontroller.h>

#include <ugdk/graphic/primitivesetup.h>


namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

void PrimitiveController::ChangePosition(const math::Vector2D& position) {
    VertexDataManipulation::ApplyPositionOffset(*owner_->vertexdata(), position - position_);
    position_ = position;
}

}  // namespace graphic
}  // namespace ugdk
