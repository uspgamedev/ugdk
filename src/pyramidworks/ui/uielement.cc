
#include <pyramidworks/ui/uielement.h>

#include <pyramidworks/ui/menu.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/util.h>

#include <string>
#include <cstdio>

namespace pyramidworks {
namespace ui {

UIElement::UIElement() : owner_(nullptr), node_(new ugdk::graphic::Node) {}

UIElement::~UIElement() { delete node_; }

} // namespace ui
} // namespace pyramidworks
