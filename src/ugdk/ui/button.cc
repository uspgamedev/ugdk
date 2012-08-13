#include <ugdk/ui/button.h>

#include <ugdk/graphic/drawable.h>
#include <ugdk/ui/menu.h>
#include <ugdk/util/intervalkdtree.h>

namespace ugdk {
namespace ui {

Button::Button(const ugdk::math::Vector2D& top_left, graphic::Drawable* drawable, const ButtonCallback& function)
  : UIElement(),
    function_(function),
    top_left_(top_left) {
        node_->set_drawable(drawable);
        node_->modifier().set_offset(top_left_);
}

Button::~Button() {
    delete node_;
}

ikdtree::Box<2> Button::GetBoundingBox() const {
    if (!node_->drawable())
        return ikdtree::Box<2>(ugdk::math::Vector2D(0.0, 0.0).val, ugdk::math::Vector2D(0.0, 0.0).val);
    ugdk::math::Vector2D top_left = owner_->node()->modifier().offset() - node_->drawable()->hotspot();
    top_left += top_left_;
    ugdk::math::Vector2D bottom_right = top_left + node_->drawable()->size();
    return ikdtree::Box<2>(top_left.val, bottom_right.val);
}

} // namespace ui
} // namespace ugdk
