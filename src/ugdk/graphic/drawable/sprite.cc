#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/action/animation.h>

namespace ugdk {
namespace graphic {

Sprite::Sprite(const Spritesheet *spritesheet, action::AnimationSet *set) 
    : spritesheet_(spritesheet), animation_manager_(new action::AnimationManager(10, set)) {}/*TODO: MANO TEM UM 10 NO MEU CÓDIGO */


Sprite::Sprite(const std::string& spritesheet_tag, action::AnimationSet *set)
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)), 
      animation_manager_(new action::AnimationManager(10, set)) {}/*TODO: MANO TEM OUTRO 10 NO MEU CÓDIGO */

Sprite::Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag) 
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)),
      animation_manager_(new action::AnimationManager(10,
             base::ResourceManager::GetAnimationSetFromFile(animation_set_tag))) {}/*TODO: MANO TEM MAIS UM 10 NO MEU CÓDIGO */


Sprite::Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag)
  : spritesheet_(spritesheet), animation_manager_(new action::AnimationManager(10,
                  base::ResourceManager::GetAnimationSetFromFile(animation_set_tag))) {}/*TODO: MANO TEM... Ah, voce entendeu */
 
Sprite::~Sprite() {
    if (animation_manager_) delete animation_manager_;
}

void Sprite::Update(double delta_t) {
    animation_manager_->Update(delta_t);
}

void Sprite::Draw() const {
    if(spritesheet_) {
        int frame_number = animation_manager_->GetFrame();

        const Modifier *animation_mod = animation_manager_->get_current_modifier();
        if(animation_mod) VIDEO_MANAGER()->PushAndApplyModifier(animation_mod);
        spritesheet_->Draw(frame_number, hotspot_);
        if(animation_mod) VIDEO_MANAGER()->PopModifier();
    }
}

const Vector2D& Sprite::size() const {
    return spritesheet_->frame_size(animation_manager_->GetFrame()); // TODO: requires some info from the spritesheet
}

}  // namespace graphic
}  // namespace ugdk
