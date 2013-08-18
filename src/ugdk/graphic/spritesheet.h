#ifndef UGDK_GRAPHIC_SPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_H_

#include <string>
#include <vector>
#include <memory>

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace graphic {

class SpritesheetData;
class Spritesheet {
  public:
    class Frame : public util::Uncopyable {
      public:
        Frame(Texture* texure, const math::Vector2D& size, const math::Vector2D& hotspot);
        ~Frame();
#ifndef SWIG
        Frame(Frame&&);
        Frame& operator=(Frame&&);
#endif

        std::unique_ptr<Texture> texture;
        math::Vector2D size, hotspot;
      private:
#ifdef _MSC_VER
        Frame(const Frame&);
        Frame& operator=(const Frame&);
#else
        Frame(const Frame&) = delete;
        Frame& operator=(const Frame&) = delete;
#endif
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
