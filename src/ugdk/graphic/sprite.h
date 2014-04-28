#ifndef UGDK_GRAPHIC_SPRITE_H_
#define UGDK_GRAPHIC_SPRITE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>

#include <ugdk/graphic.h>
#include <ugdk/graphic/primitivecontroller.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>

namespace ugdk {
namespace graphic {

class PrimitiveControllerSprite : public PrimitiveController {
  public:
    PrimitiveControllerSprite(const Spritesheet *spritesheet);
    ~PrimitiveControllerSprite();

    void ChangeToFrame(const action::SpriteAnimationFrame& frame);
       
  private:
    const Spritesheet *spritesheet_;
};


}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITE_H_
