#ifndef UGDK_GRAPHIC_SPRITESHEET_FIXEDSPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_FIXEDSPRITESHEET_H_

#include <string>
#include <vector>
#include <utility>

#include <ugdk/config/config.h>
#include "SDL_image.h"
#include "SDL_opengl.h"

#include <ugdk/base/types.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/videomanager.h>

#ifdef SWIG
#pragma SWIG nowarn=312
#endif

namespace ugdk {
namespace graphic {

/// Container for an unoptimized FixedSpritesheet data. May freely add new frames.
class FixedSpritesheetData {
  public:
    struct SpritesheetFrame {
        SDL_Surface* surface;
        Vector2D hotspot;

        SpritesheetFrame(SDL_Surface* _surface, const Vector2D& _hotspot)
            : surface(_surface), hotspot(_hotspot) {}
    };

    /// Creates a new FixedSpritesheetData from the given file.
    FixedSpritesheetData(const std::string& filename);
    ~FixedSpritesheetData();
    
    /// Adds a new frame to the frame list.
    void AddFrame(int topleft_x, int topleft_y, int width, int height, const Vector2D& hotspot);
    
    void FillWithFramesize(int width, int height, const Vector2D& hotspot);

    /// Getter for the frame data.
    const std::vector<SpritesheetFrame>& frames() const { return frames_; }
  private:
    SDL_Surface* file_data_;
    std::vector<SpritesheetFrame> frames_;
};

class FixedSpritesheet : public Spritesheet {
  public:
    /// Converts the given FixedSpritesheetData into an optimized FixedSpritesheet. 
    FixedSpritesheet(FixedSpritesheetData& data);
    virtual ~FixedSpritesheet();

    int frame_count() const {
        return frame_sizes_.size();
    }
    const Vector2D& frame_size(int frame_number) const {
        return frame_sizes_[frame_number];
    }

    // Draws at position, a draw_size square with the given frame_number
    // modified by mirror and both the image and given color and alpha.
    void Draw(int frame_number, const Vector2D& hotspot);

  private:
    void CreateList(GLuint id, Texture* texture, const Vector2D& hotspot);

    GLuint lists_base_;

    std::vector<Texture*> frames_;
    std::vector<Vector2D> frame_sizes_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
