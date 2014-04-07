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

void ApplyPositionOffset(VertexData& data, const math::Vector2D& offset);
void SpriteDataSetToGeometry(VertexData& data, const math::Vector2D& position, const action::SpriteAnimationFrame&, const Spritesheet::Frame&);

std::shared_ptr<VertexData> CreateSpriteCompatibleVertexData();
std::tuple< 
    std::shared_ptr<Primitive>, 
    std::shared_ptr<action::SpriteAnimationPlayer>
> CreateSpritePrimitive(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table);
std::shared_ptr<action::SpriteAnimationPlayer> CreateSpriteAnimationPlayerForPrimitive(const std::shared_ptr<Primitive>&, const action::SpriteAnimationTable* table);

class Sprite : public PrimitiveControllerPosition {
  public:

    Sprite(const Spritesheet *spritesheet);
    ~Sprite();

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
