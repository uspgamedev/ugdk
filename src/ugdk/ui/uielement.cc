#include <functional>
#include <string>

#include <ugdk/ui/uielement.h>
#include <ugdk/graphic/node.h>
#include <ugdk/util.h>
#include <ugdk/ui/menu.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/intervalkdtree.h>

namespace ugdk {
namespace ui {

UIElement::UIElement(const Vector2D& top_left, const Vector2D& bottom_right, Menu* owner, UICallback function)
    : top_left_(top_left), 
      bottom_right_(bottom_right),
      owner_(owner),
      function_(function),
      node_(new graphic::Node) {
          node_->modifier()->set_offset(top_left_);
}

UIElement::~UIElement() {
    delete node_;
}

ikdtree::Box<2> UIElement::GetBoundingBox() const {
    Vector2D top_left = owner_->interface_node()->modifier()->offset();
    Vector2D bottom_right = top_left + bottom_right_;
    top_left += top_left_;
    return ikdtree::Box<2>(top_left.val, bottom_right.val);
}

} // namespace ui
} // namespace ugdk
