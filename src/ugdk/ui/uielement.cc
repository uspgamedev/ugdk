#include <functional>
#include <string>
#include <stdio.h>

#include <ugdk/ui/uielement.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/util.h>
#include <ugdk/ui/menu.h>

namespace ugdk {
namespace ui {

UIElement::UIElement(const Vector2D& top_left, Menu* owner, UICallback function)
    : top_left_(top_left), 
      owner_(owner),
      function_(function),
      node_(new graphic::Node) {
          node_->modifier()->set_offset(top_left_);
}

UIElement::~UIElement() {
    delete node_;
}

ikdtree::Box<2> UIElement::GetBoundingBox() const {
    if (!node_->drawable())
        return ikdtree::Box<2>(Vector2D(0.0, 0.0).val, Vector2D(0.0, 0.0).val);
    Vector2D top_left = owner_->node()->modifier()->offset() - node_->drawable()->hotspot();
    top_left += top_left_;
    Vector2D bottom_right = top_left + node_->drawable()->size();
    return ikdtree::Box<2>(top_left.val, bottom_right.val);
}

} // namespace ui
} // namespace ugdk
