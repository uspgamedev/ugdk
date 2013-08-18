#include <ugdk/graphic/drawable/sprite.h>

#include <ugdk/resource/module.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/manager.h>
#include <ugdk/graphic/visualeffect.h>
#include <ugdk/graphic/drawable/functions.h>
#include <ugdk/action/spriteanimationframe.h>
#include <ugdk/action/animationplayer.h>

namespace ugdk {
namespace graphic {

using action::SpriteAnimationPlayer;

Sprite::Sprite(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table) 
    : spritesheet_(spritesheet), animation_player_(table) {}

Sprite::Sprite(const std::string& spritesheet_tag, const action::SpriteAnimationTable* table)
    : spritesheet_(resource::GetSpritesheetFromTag(spritesheet_tag)), 
      animation_player_(table) {}

Sprite::Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag) 
    : spritesheet_(resource::GetSpritesheetFromTag(spritesheet_tag)),
      animation_player_(resource::GetSpriteAnimationTableFromFile(animation_set_tag)) {}

Sprite::Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag)
    : spritesheet_(spritesheet),
      animation_player_(resource::GetSpriteAnimationTableFromFile(animation_set_tag)) {}
 
Sprite::~Sprite() {}

void Sprite::Draw(const Geometry& geometry, const VisualEffect& effect) const {
    if(!spritesheet_) return;
    const action::SpriteAnimationFrame& animation_frame(current_animation_frame());
    const Spritesheet::Frame& spritesheet_frame = spritesheet_->frame(animation_frame.spritesheet_frame());
    
    if(draw_setup_function_) draw_setup_function_(this, geometry, effect);

    math::Vector2D mirror_scale(
        (animation_frame.mirror() & ugdk::MIRROR_HFLIP) ? -1.0 : 1.0,
        (animation_frame.mirror() & ugdk::MIRROR_VFLIP) ? -1.0 : 1.0);

    DrawSquare(
        geometry 
            * animation_frame.geometry()
            * Geometry(math::Vector2D(), mirror_scale)
            * Geometry(-(hotspot_ + spritesheet_frame.hotspot), spritesheet_frame.size),
        effect * animation_frame.effect(),
        spritesheet_frame.texture.get());
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
