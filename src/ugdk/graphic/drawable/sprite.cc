#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/action/animation.h>

namespace ugdk {
namespace graphic {

Sprite::Sprite(Spritesheet *spritesheet, AnimationSet *set) 
    : spritesheet_(spritesheet), animation_manager_(new AnimationManager(10, set)) {}/*TODO: MANO TEM UM 10 NO MEU CÓDIGO */

Sprite::~Sprite() {
    if (animation_manager_) delete animation_manager_;
}

void Sprite::Draw(float dt) {
    if(spritesheet_) {
        int frame_number = animation_manager_->GetFrame();

        const Modifier *animation_mod = animation_manager_->get_current_modifier();
        if(animation_mod) VIDEO_MANAGER()->PushAndApplyModifier(animation_mod);
        spritesheet_->Draw(frame_number, hotspot_);
        if(animation_mod) VIDEO_MANAGER()->PopModifier();
    }
    Update(dt);
}

void Sprite::Update(float delta_t) {
    animation_manager_->Update(delta_t);
}

const Vector2D& Sprite::size() const {
    return spritesheet_->frame_size(animation_manager_->GetFrame()); // TODO: requires some info from the spritesheet
}

}  // namespace graphic
}  // namespace ugdk
