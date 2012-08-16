#ifndef HORUSEYE_FRAMEWORK_SPRITE_H_
#define HORUSEYE_FRAMEWORK_SPRITE_H_

#include <string>

#include <ugdk/math/vector2D.h>
#include <ugdk/action.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>

namespace ugdk {
namespace action {
typedef ugdk::action::SpecializedAnimationManager<ugdk::action::SpriteAnimation> SpriteAnimationManager;
}
namespace graphic {

class Sprite : public Drawable {
  public:
    Sprite(const Spritesheet *spritesheet, action::SpriteAnimationManager *manager = NULL);
    explicit Sprite(const std::string& spritesheet_tag, action::SpriteAnimationManager *manager = NULL);
    //explicit Sprite(const std::string& spritesheet_tag, const std::string& animation_set_tag);
    //explicit Sprite(const Spritesheet *spritesheet, const std::string& animation_set_tag);
    virtual ~Sprite();

    void Update(double dt);
    void Draw() const;
    const ugdk::math::Vector2D& size() const;

    const action::SpriteAnimationManager* animation_manager() { return animation_manager_; }
       
  private:
    const Spritesheet *spritesheet_;
    action::SpriteAnimationManager *animation_manager_;
};

}  // namespace graphic
}  // namespace ugdk

#endif //HORUSEYE_FRAMEWORK_SPRITE_H_
