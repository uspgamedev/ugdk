#ifndef HORUSEYE_FRAMEWORK_DRAWABLE_H_
#define HORUSEYE_FRAMEWORK_DRAWABLE_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/modifier.h>

namespace ugdk {

class Drawable {
  public:
    virtual void Draw() = 0;

    virtual Vector2D render_size() const = 0;

    // DEPRECATED DO NOT USE OR DIE LOLOL
    virtual int width() const { return (int)(render_size().x); }
    virtual int height() const { return (int)(render_size().y); }
    virtual bool Destroy() { return false; }

  protected:
    Drawable() {}
};

}  // namespace framework

#endif
