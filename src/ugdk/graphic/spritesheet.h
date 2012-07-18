#ifndef UGDK_GRAPHIC_SPRITESHEET_SPRITESHEET_H_
#define UGDK_GRAPHIC_SPRITESHEET_SPRITESHEET_H_

#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace graphic {

class Spritesheet {
  public:
    virtual ~Spritesheet() {}

    virtual void Draw(int frame_number, const Vector2D& hotspot) = 0;
    
    virtual             int frame_count()                const = 0;
    virtual const Vector2D& frame_size(int frame_number) const = 0;

  protected:
    Spritesheet() {}
};

}  // namespace graphic
}  // namespace ugdk

#endif
