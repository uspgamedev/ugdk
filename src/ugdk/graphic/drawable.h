#ifndef HORUSEYE_FRAMEWORK_DRAWABLE_H_
#define HORUSEYE_FRAMEWORK_DRAWABLE_H_

#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace graphic {

class Drawable {
  public:
    enum HookPoint {
        TOP_LEFT,       TOP,        TOP_RIGHT,
        LEFT,           CENTER,     RIGHT,
        BOTTOM_LEFT,    BOTTOM,     BOTTOM_RIGHT
    };
    virtual ~Drawable() {};

    virtual void Update(double dt) = 0;
    virtual void Draw() const = 0;
    virtual const Vector2D& size() const = 0;

    void set_hotspot(const Vector2D& hotspot) { hotspot_ = hotspot; }
    void set_hotspot(const HookPoint& hook) {
        switch(hook) {
        case TOP_LEFT    : hotspot_ = Vector2D(           0.0,            0.0); break;
        case TOP         : hotspot_ = Vector2D(size().x * 0.5,            0.0); break;
        case TOP_RIGHT   : hotspot_ = Vector2D(       size().x,            0.0); break;
        case LEFT        : hotspot_ = Vector2D(           0.0, size().y * 0.5); break;
        case CENTER      : hotspot_ = size() * 0.5;                              break;
        case RIGHT       : hotspot_ = Vector2D(       size().x, size().y * 0.5); break;
        case BOTTOM_LEFT : hotspot_ = Vector2D(           0.0,        size().y); break;
        case BOTTOM      : hotspot_ = Vector2D(size().x * 0.5,        size().y); break;
        case BOTTOM_RIGHT: hotspot_ = size();                                     break;
        }
    }

    const double       width() const { return size().x; }
    const double      height() const { return size().y; }
    const Vector2D& hotspot() const { return hotspot_; }

  protected:
    Drawable() {}

    ugdk::Vector2D hotspot_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
