#ifndef UGDK_GRAPHIC_FLEXIBLESPRITESHEET_H_
#define UGDK_GRAPHIC_FLEXIBLESPRITESHEET_H_

#include <string>
#include <SDL/SDL.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>

namespace ugdk {

// Square with texture implementation of a Drawable.
class FlexibleSpritesheet : public Spritesheet {
  public:
    FlexibleSpritesheet();
	virtual ~FlexibleSpritesheet();

	int width() const { return texture_width_; }
    int height() const { return texture_height_; }
    const Vector2D render_size() const { return Vector2D((float)texture_width_, (float)texture_height_); }

	// Sets the frame_size to use with the texture.
    // 0.0f <= size.x,size.y <= 1.0f
    void set_frame_size(const Vector2D& size) { 
        frame_size_ = size;
        if(frame_size_.x > 1 || frame_size_.y > 1) {
            frame_size_.x /= texture_width_;
            frame_size_.y /= texture_height_;
        }
    }
    const Vector2D& frame_size() const { return frame_size_; }
    
    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }
    const Vector2D& hotspot() const { return hotspot_; }
    
	int FrameCount() const;

	// Draws at position, a draw_size square with the given frame_number
	// modified by mirror and both the image and given color and alpha.
    void Draw(int frame_number);

	// Loads the image at filepath and creates a texture from the data.
    bool LoadFromFile(const std::string& filepath);

  private:
    uint32 texture_;
    int texture_width_, texture_height_;
    Vector2D frame_size_, hotspot_;

    std::string filename_;
};

}  // namespace framework

#endif
