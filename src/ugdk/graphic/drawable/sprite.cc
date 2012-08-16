#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/base/engine.h>
//#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/specializedanimationmanager.h>

namespace ugdk {
namespace graphic {

Sprite::Sprite(const Spritesheet *spritesheet, SpriteAnimationManager *manager) 
    : spritesheet_(spritesheet), animation_manager_(manager) {}

/*
Sprite::Sprite(const std::string& spritesheet_tag, SpriteAnimationManager* manager)
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)), 
      animation_manager_(manager) {}

/*
Sprite::Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag) 
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)),
      animation_manager_(new action::AnimationManager(
             base::ResourceManager::GetAnimationSetFromFile(animation_set_tag))) {}

Sprite::Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag)
  : spritesheet_(spritesheet), animation_manager_(new action::AnimationManager(
                  base::ResourceManager::GetAnimationSetFromFile(animation_set_tag))) {}
                  */
 
Sprite::~Sprite() {
    if (animation_manager_) delete animation_manager_;
}

void Sprite::Update(double delta_t) {
    animation_manager_->Update(delta_t);
}

void Sprite::Draw() const {
    if(spritesheet_) {
        const action::SpriteAnimationFrame* animation_frame = animation_manager_->current_animation_frame();

        const Modifier *animation_mod = animation_frame->modifier(); 
        if(animation_mod) VIDEO_MANAGER()->PushAndApplyModifier(animation_mod);
        spritesheet_->Draw(animation_frame->frame(), hotspot_);
        if(animation_mod) VIDEO_MANAGER()->PopModifier();
    }
}

const ugdk::math::Vector2D& Sprite::size() const {
    return spritesheet_->frame_size(animation_manager_->current_animation_frame()->frame()); // TODO: requires some info from the spritesheet
}

}  // namespace graphic
}  // namespace ugdk
