#ifndef UGDK_GRAPHIC_SPRITE_H_
#define UGDK_GRAPHIC_SPRITE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/graphic/primitivecontroller.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include <string>

namespace ugdk {
namespace graphic {

class Sprite : public PrimitiveControllerPosition {
    class SpriteData : public VertexData {
      public:
        ~SpriteData();

        void ApplyPositionOffset(const math::Vector2D& offset);
        void SetToGeometry(const math::Vector2D& position, const math::Vector2D& size, const math::Vector2D& hotspot, const Geometry& geometry);
        void Draw(opengl::ShaderUse&) const;

      private:
        friend class Sprite;
        SpriteData();

        std::shared_ptr<opengl::VertexBuffer> position_;
        std::shared_ptr<const opengl::VertexBuffer> uv_;
    };

  public:
    static Sprite* Create(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table = nullptr);
    static Sprite* Create(const std::string& spritesheet_tag, const action::SpriteAnimationTable* table = nullptr);
    static Sprite* Create(const std::string& spritesheet_tag, const std::string& animation_set_tag);
    static Sprite* Create(const Spritesheet *spritesheet, const std::string& animation_set_tag);

    Sprite(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table);
    ~Sprite();

    std::shared_ptr<Primitive> primitive() const;
    const action::SpriteAnimationPlayer& animation_player() const;
    action::SpriteAnimationPlayer& animation_player();

    void ChangeToFrame(const action::SpriteAnimationFrame& frame);
    void ChangePosition(const math::Vector2D& position); // TODO overide
       
  private:
    std::shared_ptr<SpriteData> sprite_data_;
    const Spritesheet *spritesheet_;
    std::shared_ptr<Primitive> primitive_;
    action::SpriteAnimationPlayer animation_player_;
    math::Vector2D position_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITE_H_
