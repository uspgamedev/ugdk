#ifndef UGDK_UI_DRAWABLE_H_
#define UGDK_UI_DRAWABLE_H_

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/ui/hookpoint.h>

#include <functional>

namespace ugdk {
namespace ui {

class Drawable {
  public:
    virtual ~Drawable() {}

    virtual void Draw(graphic::Canvas&) const = 0;
    virtual const math::Vector2D& size() const = 0;

    void set_draw_setup_function(const std::function<void (const Drawable*, graphic::Canvas&)> &func) {
        draw_setup_function_ = func;
    }

    void set_hotspot(const ugdk::math::Vector2D& _hotspot) { hotspot_ = _hotspot; }
    void set_hotspot(const HookPoint& hook) {
        switch(hook) {
        case HookPoint::TOP_LEFT:       hotspot_ = ugdk::math::Vector2D(0.0, 0.0);                  break;
        case HookPoint::TOP:            hotspot_ = ugdk::math::Vector2D(size().x * 0.5, 0.0);       break;
        case HookPoint::TOP_RIGHT:      hotspot_ = ugdk::math::Vector2D(size().x, 0.0);             break;
        case HookPoint::LEFT:           hotspot_ = ugdk::math::Vector2D(0.0, size().y * 0.5);       break;
        case HookPoint::CENTER:         hotspot_ = size() * 0.5;                                    break;
        case HookPoint::RIGHT:          hotspot_ = ugdk::math::Vector2D(size().x, size().y * 0.5);  break;
        case HookPoint::BOTTOM_LEFT:    hotspot_ = ugdk::math::Vector2D(0.0, size().y);             break;
        case HookPoint::BOTTOM:         hotspot_ = ugdk::math::Vector2D(size().x * 0.5, size().y);  break;
        default:                        hotspot_ = size(); /* Matches BOTTOM_RIGHT */               break;
        }
    }

    double       width() const { return size().x; }
    double      height() const { return size().y; }
    const math::Vector2D& hotspot() const { return hotspot_; }

  protected:
    Drawable() {}

    math::Vector2D hotspot_;
    std::function<void (const Drawable*, graphic::Canvas&)> draw_setup_function_;
};

}  // namespace ui
}  // namespace ugdk

#endif // UGDK_UI_DRAWABLE_H_
