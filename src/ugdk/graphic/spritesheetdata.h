#ifndef UGDK_GRAPHIC_SPRITESHEETDATA_H_
#define UGDK_GRAPHIC_SPRITESHEETDATA_H_

#include <string>
#include <list>
#include <vector>
#include <utility>

#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#ifdef SWIG
#pragma SWIG nowarn=325
#endif

namespace ugdk {
namespace graphic {

struct PixelSurface;

/// Container for an unoptimized FixedSpritesheet data. May freely add new frames.
class SpritesheetData {
  public:
    struct SpritesheetFrame {
        PixelSurface* surface;
        ugdk::math::Vector2D hotspot;

        SpritesheetFrame(PixelSurface* _surface, const ugdk::math::Vector2D& _hotspot)
            : surface(_surface), hotspot(_hotspot) {}
        ~SpritesheetFrame();
    };

    /// Creates a new FixedSpritesheetData from the given file.
    SpritesheetData(const std::string& filename);
    SpritesheetData(const std::list<std::string>& filenames);
    ~SpritesheetData();
    
    /// Adds a new frame to the frame list.
    void AddFrame(int topleft_x, int topleft_y, int width, int height, const ugdk::math::Vector2D& hotspot, size_t file = 0);
    
    /// Adds frames of equal size, all with the same hotspot.
    void FillWithFramesize(int width, int height, const ugdk::math::Vector2D& hotspot, size_t file = 0);

    /// Calls FillWithFramesize for all files.
    void FillWithFramesizeFromAllFiles(int width, int height, const ugdk::math::Vector2D& hotspot);

    /// Getter for the frame data.
    const std::list<SpritesheetFrame>& frames() const { return frames_; }

  private:
    std::vector<PixelSurface*> file_data_;
    std::list<SpritesheetFrame> frames_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITESHEETDATA_H_