#ifndef HORUSEYE_FRAMEWORK_DRAWABLE_H_
#define HORUSEYE_FRAMEWORK_DRAWABLE_H_

#include <ugdk/math/vector2D.h>

namespace ugdk {

class Drawable {
  public:
    virtual ~Drawable() {};

    virtual void Draw(float dt) = 0;

    virtual const Vector2D& size() const = 0;

    float  width() const { return size().x; }
    float height() const { return size().y; }

  protected:
    Drawable() {}
};

}  // namespace framework

#endif
