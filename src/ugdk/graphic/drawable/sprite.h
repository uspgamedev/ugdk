#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <string>

#include <ugdk/math/vector2D.h>
#include <ugdk/action.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>

namespace ugdk {
namespace graphic {

class Sprite : public Drawable {
  public:
    Sprite(const Spritesheet *spritesheet, action::SpriteAnimationPlayer *manager = NULL);
    explicit Sprite(const std::string& spritesheet_tag, action::SpriteAnimationPlayer *manager = NULL);
    explicit Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag);
    explicit Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag);
    virtual ~Sprite();

    void Update(double dt);
    void Draw(const Geometry&, const VisualEffect&) const;
    const ugdk::math::Vector2D& size() const;

    const action::SpriteAnimationFrame* current_animation_frame() const;
    const action::SpriteAnimationPlayer* animation_player() const;
       
  private:
    const Spritesheet *spritesheet_;
    action::SpriteAnimationPlayer *animation_player_;
};

}  // namespace graphic
}  // namespace ugdk

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
