#ifndef UGDK_UI_UIELEMENT_H_
#define UGDK_UI_UIELEMENT_H_

#include <ugdk/structure.h>
#include <ugdk/ui.h>

#include <memory>

namespace ugdk {
namespace ui {

class UIElement {
  public:
    virtual ~UIElement();
    
    void set_owner(Menu* owner) { owner_ = owner; }

    Menu* owner() const { return owner_; }
    std::shared_ptr<Node> node() const { return node_; }

    virtual void Interact() const = 0;
    virtual ugdk::structure::Box<2> GetBoundingBox() const = 0;

  protected:
    UIElement();

  private:
    Menu* owner_;
    std::shared_ptr<Node> node_;
};

} // namespace ugdk
} // namespace ugdk

#endif // UGDK_UI_UIELEMENT_H_
