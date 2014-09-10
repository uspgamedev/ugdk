
#include <ugdk/ui/uielement.h>

#include <ugdk/ui/menu.h>
#include <ugdk/ui/node.h>
#include <ugdk/util.h>

#include <string>
#include <cstdio>

namespace ugdk {
namespace ui {

UIElement::UIElement()
:  owner_(nullptr)
,  node_(new Node)
{}

UIElement::~UIElement() {}

} // namespace ui
} // namespace ugdk
