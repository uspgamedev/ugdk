#ifndef UGDK_UI_UIELEMENT_H_
#define UGDK_UI_UIELEMENT_H_

#include <ugdk/util.h>
#include <ugdk/structure.h>
#include <ugdk/graphic.h>
#include <ugdk/ui.h>

namespace ugdk {
namespace ui {

class UIElement {
  public:
    virtual ~UIElement();
    
    void set_owner(Menu* owner) { owner_ = owner; }
    ugdk::graphic::Node* node() const { return node_; }

    virtual void Interact() const = 0;
    virtual ugdk::structure::Box<2> GetBoundingBox() const = 0;

  protected:
    UIElement();

    Menu* owner_;
    ugdk::graphic::Node* node_;
};

} // namespace ugdk
} // namespace ugdk

#endif // UGDK_UI_UIELEMENT_H_
