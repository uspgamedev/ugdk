
#include <ugdk/ui/uielement.h>

#include <ugdk/ui/menu.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/util.h>

#include <string>
#include <cstdio>

namespace ugdk {
namespace ui {

UIElement::UIElement() : owner_(nullptr), node_(new ugdk::graphic::Node) {}

UIElement::~UIElement() { delete node_; }

} // namespace ui
} // namespace ugdk
