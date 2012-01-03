#ifndef HORUSEYE_FRAMEWORK_DRAWABLE_H_
#define HORUSEYE_FRAMEWORK_DRAWABLE_H_

#include <ugdk/math/vector2D.h>

namespace ugdk {

class Drawable {
  public:
    virtual ~Drawable() {};

    virtual void Draw() = 0;

    virtual const Vector2D& size() const = 0;
    Vector2D render_size() const { return size(); }

    // DEPRECATED DO NOT USE OR DIE LOLOL
    virtual int width() const { return (int)(render_size().x); }
    virtual int height() const { return (int)(render_size().y); }

  protected:
    Drawable() {}
};

}  // namespace framework

#endif
