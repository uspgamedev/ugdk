#include <ugdk/ui/button.h>

#include <ugdk/ui/drawable.h>
#include <ugdk/ui/menu.h>
#include <ugdk/ui/node.h>
#include <ugdk/structure/intervalkdtree.h>

namespace ugdk {
namespace ui {

Button::Button(const ugdk::math::Vector2D& top_left, std::unique_ptr<Drawable>&& drawable, const ButtonCallback& function)
  : function_(function)
  , top_left_(top_left)
{
    node()->set_drawable(std::move(drawable));
    node()->geometry().ChangeOffset(top_left_);
}

Button::~Button() {}

structure::Box<2> Button::GetBoundingBox() const {
    if (!node()->drawable())
        return structure::Box<2>(ugdk::math::Vector2D(0.0, 0.0), ugdk::math::Vector2D(0.0, 0.0));

    ugdk::math::Vector2D top_left = owner()->node()->geometry().offset() - node()->drawable()->hotspot();
    top_left += top_left_;
    ugdk::math::Vector2D bottom_right = top_left + node()->drawable()->size();
    return structure::Box<2>(top_left, bottom_right);
}

} // namespace ui
} // namespace ugdk
