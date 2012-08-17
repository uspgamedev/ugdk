#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/base/engine.h>
//#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

Sprite::Sprite(const Spritesheet *spritesheet, SpriteAnimationPlayer *player) 
    : spritesheet_(spritesheet), animation_player_(player) {}

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
    if (animation_player_) delete animation_player_;
}

void Sprite::Update(double delta_t) {
    animation_player_->Update(delta_t);
}

void Sprite::Draw() const {
    if(spritesheet_) {
        const action::SpriteAnimationFrame* animation_frame = 
            current_animation_frame();

        const Modifier *animation_mod = animation_frame->modifier(); 
        if(animation_mod) VIDEO_MANAGER()->PushAndApplyModifier(animation_mod);
        spritesheet_->Draw(animation_frame->frame(), hotspot_);
        if(animation_mod) VIDEO_MANAGER()->PopModifier();
    }
}

const ugdk::math::Vector2D& Sprite::size() const {
    return spritesheet_->frame_size(current_animation_frame()->frame()); // TODO: requires some info from the spritesheet
}
    
const action::SpriteAnimationFrame* Sprite::current_animation_frame() const {
    return animation_player_->current_animation_frame();
}
    
const action::SpriteAnimationPlayer* Sprite::animation_player() const { 
    return animation_player_;
}

}  // namespace graphic
}  // namespace ugdk
