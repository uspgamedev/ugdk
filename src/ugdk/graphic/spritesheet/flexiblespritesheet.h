#ifndef UGDK_GRAPHIC_SPRITESHEET_FLEXIBLESPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_FLEXIBLESPRITESHEET_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/spritesheet.h>

namespace ugdk {
namespace graphic {

// A spritesheet with a variable frame size.
class FlexibleSpritesheet : public Spritesheet {
  public:
    FlexibleSpritesheet(Texture* texture);
    virtual ~FlexibleSpritesheet();

    const Vector2D render_size() const;

    // Sets the frame_size to use with the texture.
    // 0.0f <= size.x,size.y <= 1.0f
    void set_frame_size(const Vector2D& size);
    const Vector2D& frame_size() const { return frame_size_; }
    
    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }
    const Vector2D& hotspot() const { return hotspot_; }
    
                int frame_count()                const;
    const Vector2D& frame_size(int frame_number) const {
        return real_frame_size_;
    }

    // Draws at position, a draw_size square with the given frame_number
    // modified by mirror and both the image and given color and alpha.
    void Draw(int frame_number, const Vector2D& hotspot);

  private:
    Texture* texture_;
    Vector2D frame_size_, hotspot_, real_frame_size_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
