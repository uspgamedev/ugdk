#ifndef UGDK_GRAPHIC_DRAWABLE_SPRITESHEET_H_
#define UGDK_GRAPHIC_DRAWABLE_SPRITESHEET_H_

#include <ugdk/math/vector2D.h>

namespace ugdk {

class Spritesheet {
  public:
    virtual ~Spritesheet() {}

    virtual void Draw(int frame_number, const Vector2D& hotspot) = 0;
	virtual int FrameCount() const = 0;

    virtual const Vector2D& GetFrameSize(int frame_number) const = 0;
    
    virtual void set_hotspot(const Vector2D& hotspot) = 0;
    virtual const Vector2D& hotspot() const = 0;

  protected:
    Spritesheet() {}
};

}  // namespace framework

#endif
