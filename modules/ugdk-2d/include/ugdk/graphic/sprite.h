#ifndef UGDK_GRAPHIC_SPRITE_H_
#define UGDK_GRAPHIC_SPRITE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>

#include <ugdk/action/animationplayer.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/primitivecontroller.h>
#include <ugdk/graphic/textureatlas.h>

namespace ugdk {
namespace graphic {

class PrimitiveControllerSprite : public PrimitiveController {
  public:
    PrimitiveControllerSprite(const TextureAtlas* spritesheet);
    ~PrimitiveControllerSprite();

    void ChangeToAnimationFrame(const action::SpriteAnimationFrame& frame);
    void ChangeToAtlasFrame(const std::string& frame_name);
    void ChangeToAtlasFrame(std::size_t frame_number);
       
  private:
    void ChangeToBoundPiece(TextureAtlas::BoundPiece& piece);

    const TextureAtlas* spritesheet_;
};


}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITE_H_
