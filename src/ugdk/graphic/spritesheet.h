#ifndef UGDK_GRAPHIC_SPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_H_

#include <string>
#include <vector>
#include <memory>

#include <ugdk/graphic.h>
#include <ugdk/graphic/textureatlas.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/uncopyable.h>

namespace ugdk {
namespace graphic {

class Spritesheet {
  public:
    class Frame {
      public:
        Frame(const TextureAtlas& atlas, size_t index, const math::Vector2D& hotspot);
        ~Frame();

        TextureAtlas::BoundPiece piece;
        math::Vector2D hotspot;
    };

    Spritesheet(const std::shared_ptr<TextureAtlas>& atlas, const std::vector< std::pair<size_t, math::Vector2D> >& frames);
    ~Spritesheet();

    size_t frame_count() const {
        return frames_.size();
    }

    const Frame& frame(size_t frame_number) const {
        return frames_[frame_number];
    }

    std::shared_ptr<TextureAtlas> atlas() const { return atlas_; }
    const ugdk::math::Vector2D& frame_size(size_t frame_number) const;

  private:
    std::shared_ptr<TextureAtlas> atlas_;
    std::vector<Frame> frames_;
};

Spritesheet* CreateSpritesheetFromTag(const std::string&);

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_SPRITESHEET_H_
