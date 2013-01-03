#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

Sprite::Sprite(const Spritesheet *spritesheet, SpriteAnimationPlayer *player) 
    : spritesheet_(spritesheet), animation_player_(player) {}

Sprite::Sprite(const std::string& spritesheet_tag, SpriteAnimationPlayer* manager)
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)), 
      animation_player_(manager) {}

Sprite::Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag) 
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)),
      animation_player_(new action::SpriteAnimationPlayer(
             base::ResourceManager::GetSpriteAnimationTableFromFile(animation_set_tag))) {}

Sprite::Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag)
  : spritesheet_(spritesheet), animation_player_(new action::SpriteAnimationPlayer(
                  base::ResourceManager::GetSpriteAnimationTableFromFile(animation_set_tag))) {}
 
Sprite::~Sprite() {
    if (animation_player_) delete animation_player_;
}

void Sprite::Update(double delta_t) {
    if(animation_player_) animation_player_->Update(delta_t);
}

void Sprite::Draw() const {
    if(!spritesheet_) return;
    if(animation_player_) {
        const action::SpriteAnimationFrame* animation_frame = 
            current_animation_frame();

        const Modifier& animation_mod = animation_frame->modifier(); 
        VIDEO_MANAGER()->PushAndApplyModifier(animation_mod);
        spritesheet_->Draw(animation_frame->frame(), hotspot_);
        VIDEO_MANAGER()->PopModifier();
    } else {
        spritesheet_->Draw(0, hotspot_);
    }
}

const ugdk::math::Vector2D& Sprite::size() const {
    return spritesheet_->frame_size(current_animation_frame()->frame()); // TODO: requires some info from the spritesheet
}
    
const action::SpriteAnimationFrame* Sprite::current_animation_frame() const {
    return animation_player_ ? animation_player_->current_animation_frame() : NULL;
}
    
const action::SpriteAnimationPlayer* Sprite::animation_player() const { 
    return animation_player_;
}

void Sprite::Select(const std::string& name) {
    if(animation_player_) animation_player_->Select(name);
}

void Sprite::Select(int index) {
    if(animation_player_) animation_player_->Select(index);
}

}  // namespace graphic
}  // namespace ugdk
