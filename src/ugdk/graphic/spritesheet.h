#ifndef UGDK_GRAPHIC_SPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_H_

#include <string>
#include <list>
#include <vector>
#include <utility>

#include <ugdk/base/types.h>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#ifdef SWIG
#pragma SWIG nowarn=325
#endif
#ifndef __GL_H__
extern "C" {
typedef unsigned int GLuint;
}
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

class Spritesheet {
  public:
    /// Converts the given FixedSpritesheetData into an optimized FixedSpritesheet. 
    Spritesheet(const SpritesheetData& data);
    virtual ~Spritesheet();

    size_t frame_count() const {
        return frames_.size();
    }

    const ugdk::math::Vector2D& frame_size(size_t frame_number) const;

    /** Draws at position, a draw_size square with the given frame_number
        modified by mirror and both the image and given color and alpha. */
    void Draw(int frame_number, const ugdk::math::Vector2D& hotspot, const Geometry&, const VisualEffect&) const;

  private:
    opengl::VertexBuffer* createBuffer();
    struct Frame {
        Frame(Texture* _tex, const math::Vector2D& _size, const math::Vector2D& _hotspot)
            : texture(_tex), size(_size), hotspot(_hotspot) {}

        Texture* texture;
        math::Vector2D size, hotspot;
    };

    std::vector<Frame> frames_;
    opengl::VertexBuffer* vertexbuffer_;
    opengl::VertexBuffer* uvbuffer_;
};

Spritesheet* CreateSpritesheetFromTag(const std::string&);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITESHEET_H_
