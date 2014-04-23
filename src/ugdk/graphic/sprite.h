#ifndef UGDK_GRAPHIC_SPRITE_H_
#define UGDK_GRAPHIC_SPRITE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/primitivecontroller.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/system/compatibility.h>

#include <string>
#include <utility>

namespace ugdk {
namespace graphic {

class PrimitiveControllerSprite : public PrimitiveControllerPosition {
  public:

    PrimitiveControllerSprite(const Spritesheet *spritesheet);
    ~PrimitiveControllerSprite();

    void set_owner(Primitive*) override;
    void ChangeToFrame(const action::SpriteAnimationFrame& frame);
    void ChangePosition(const math::Vector2D& position) override;
       
  private:

    const Spritesheet *spritesheet_;
    math::Vector2D position_;
    Primitive* owner_;
};


}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITE_H_
