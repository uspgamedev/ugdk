
#include <string>
#include <stdio.h>

#include <ugdk/ui/uielement.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/util.h>
#include <ugdk/ui/menu.h>

namespace ugdk {
namespace ui {

UIElement::UIElement() : owner_(nullptr), node_(new graphic::Node) {}

UIElement::~UIElement() { delete node_; }

} // namespace ui
} // namespace ugdk
