#include <functional>
#include <string>

#include <ugdk/ui/uielement.h>
#include <ugdk/graphic/node.h>
#include <ugdk/util.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/intervalkdtree.h>

namespace ugdk {
namespace ui {

UIElement::UIElement(const Vector2D& top_left, const Vector2D& bottom_right, UICallback function)
    : top_left_(top_left), 
      bottom_right_(bottom_right),
      function_(function),
      node_(new graphic::Node) {
          node_->modifier()->set_offset(top_left_);
}

UIElement::~UIElement() {
    delete node_;
}

} // namespace ui
} // namespace ugdk
