#ifndef UGDK_GRAPHIC_SPRITE_H_
#define UGDK_GRAPHIC_SPRITE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/primitivecontroller.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include <ugdk/system/compatibility.h>

#include <string>

namespace ugdk {
namespace graphic {

void ApplyPositionOffset(VertexData& data, const math::Vector2D& offset);
void SpriteDataSetToGeometry(VertexData& data, const math::Vector2D& position, const math::Vector2D& size, const math::Vector2D& hotspot, const Geometry& geometry);

class Sprite : public PrimitiveControllerPosition {
  public:
    typedef std::tuple< std::shared_ptr<Primitive>, std::shared_ptr<action::SpriteAnimationPlayer> > SpriteCreateTuple;
    static SpriteCreateTuple Create(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table);

    ~Sprite();

    void set_owner(Primitive*) override;
    void ChangeToFrame(const action::SpriteAnimationFrame& frame);
    void ChangePosition(const math::Vector2D& position) override;
       
  private:
    Sprite(const Spritesheet *spritesheet);

    const Spritesheet *spritesheet_;
    math::Vector2D position_;
    Primitive* owner_;
};


}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITE_H_
