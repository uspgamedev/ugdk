#ifndef UGDK_GRAPHIC_SPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_H_

#include <string>
#include <vector>

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace graphic {

class SpritesheetData;
class Spritesheet {
  public:
    struct Frame {
        Frame(Texture* _tex, const math::Vector2D& _size, const math::Vector2D& _hotspot)
            : texture(_tex), size(_size), hotspot(_hotspot) {}
        ~Frame();

        Texture* texture;
        math::Vector2D size, hotspot;
    };

    /// Converts the given SpritesheetData into an optimized Spritesheet.
    Spritesheet(const SpritesheetData& data);
    ~Spritesheet();

    size_t frame_count() const {
        return frames_.size();
    }

    const Frame& frame(size_t frame_number) const {
        return frames_[frame_number];
    }

    const ugdk::math::Vector2D& frame_size(size_t frame_number) const;

  private:
    std::vector<Frame> frames_;
};

Spritesheet* CreateSpritesheetFromTag(const std::string&);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITESHEET_H_
