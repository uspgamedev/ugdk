#ifndef PYRAMIDWORKS_UI_UIELEMENT_H_
#define PYRAMIDWORKS_UI_UIELEMENT_H_

#include <ugdk/util.h>
#include <ugdk/structure.h>
#include <ugdk/graphic.h>
#include <pyramidworks/ui.h>

namespace pyramidworks {
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

} // namespace pyramidworks
} // namespace ugdk

#endif // PYRAMIDWORKS_UI_UIELEMENT_H_
