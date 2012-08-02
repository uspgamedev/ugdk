#ifndef UGDK_UI_UIELEMENT_H_
#define UGDK_UI_UIELEMENT_H_

#include <ugdk/ui.h>
#include <ugdk/util.h>
#include <ugdk/graphic.h>

namespace ugdk {
namespace ui {

class UIElement {
  public:
    virtual ~UIElement();
    
    void set_owner(Menu* owner) { owner_ = owner; }
    graphic::Node* node() const { return node_; }

    virtual void Interact() const = 0;
    virtual ikdtree::Box<2> GetBoundingBox() const = 0;

  protected:
    UIElement();

    Menu* owner_;
    graphic::Node* node_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UIELEMENT_H_ */
