#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

Sprite::Sprite(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table) 
    : spritesheet_(spritesheet), animation_player_(table) {}

Sprite::Sprite(const std::string& spritesheet_tag, const action::SpriteAnimationTable* table)
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)), 
      animation_player_(table) {}

Sprite::Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag) 
    : spritesheet_(base::ResourceManager::GetSpritesheetFromTag(spritesheet_tag)),
      animation_player_(base::ResourceManager::GetSpriteAnimationTableFromFile(animation_set_tag)) {}

Sprite::Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag)
    : spritesheet_(spritesheet),
      animation_player_(base::ResourceManager::GetSpriteAnimationTableFromFile(animation_set_tag)) {}
 
Sprite::~Sprite() {}

void Sprite::Draw(const Geometry& modifier, const VisualEffect& effect) const {
    if(!spritesheet_) return;
    const action::SpriteAnimationFrame& animation_frame(current_animation_frame());

    math::Vector2D mirror_scale(
            (animation_frame.mirror() & ugdk::MIRROR_HFLIP) ? -1.0 : 1.0,
            (animation_frame.mirror() & ugdk::MIRROR_VFLIP) ? -1.0 : 1.0);

    spritesheet_->Draw(
        animation_frame.spritesheet_frame(), 
        hotspot_, 
        modifier * animation_frame.geometry() * Geometry(math::Vector2D(), mirror_scale),
        effect * animation_frame.effect());
}

const ugdk::math::Vector2D& Sprite::size() const {
    return spritesheet_->frame_size(current_animation_frame().spritesheet_frame());
        // TODO: requires some info from the spritesheet
}
    
const action::SpriteAnimationFrame& Sprite::current_animation_frame() const {
    return animation_player_.current_animation_frame();
}
    
const action::SpriteAnimationPlayer& Sprite::animation_player() const { 
    return animation_player_;
}

action::SpriteAnimationPlayer& Sprite::animation_player() { 
    return animation_player_;
}

}  // namespace graphic
}  // namespace ugdk
