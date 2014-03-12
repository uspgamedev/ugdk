#ifndef UGDK_GRAPHIC_SPRITE_H_
#define UGDK_GRAPHIC_SPRITE_H_

#include <ugdk/graphic.h>
#include <ugdk/action.h>

#include <ugdk/graphic/primitive.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/action/animationplayer.h>
#include <ugdk/action/spritetypes.h>
#include <string>

namespace ugdk {
namespace graphic {

class Sprite {
    class SpriteData : public VertexData {
      public:
        ~SpriteData();

        void SetToGeometry(const math::Vector2D& size, const math::Vector2D& hotspot, const Geometry& geometry);
        void Draw(opengl::ShaderUse&) const;

      private:
        friend class Sprite;
        SpriteData();

        std::shared_ptr<const opengl::VertexBuffer> position_, uv_;
    };

  public:
    static Sprite* Create(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table = nullptr);
    static Sprite* Create(const std::string& spritesheet_tag, const action::SpriteAnimationTable* table = nullptr);
    static Sprite* Create(const std::string& spritesheet_tag, const std::string& animation_set_tag);
    static Sprite* Create(const Spritesheet *spritesheet, const std::string& animation_set_tag);

    Sprite(const Spritesheet *spritesheet, const action::SpriteAnimationTable* table);
    ~Sprite();

    const Primitive& primitive() const;
    const action::SpriteAnimationPlayer& animation_player() const;
    action::SpriteAnimationPlayer& animation_player();

    void ChangeToFrame(const action::SpriteAnimationFrame& frame);
       
  private:
    std::shared_ptr<SpriteData> sprite_data_;
    const Spritesheet *spritesheet_;
    Primitive primitive_;
    action::SpriteAnimationPlayer animation_player_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITE_H_
