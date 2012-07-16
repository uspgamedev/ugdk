
#ifndef UGDK_UI_UIELEMENT_H_
#define UGDK_UI_UIELEMENT_H_

#include <ugdk/portable/tr1.h>
#include FROM_TR1(functional)
#include <string>

#include <ugdk/util.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/node.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/intervalkdtree.h>

#include <ugdk/ui.h>

namespace ugdk {
namespace ui {

class UIElement {
  public:
    typedef std::tr1::function<void (const UIElement *)> UICallback;
    
    UIElement(const Vector2D& top_left, graphic::Drawable* drawable, UICallback function);
    
    virtual ~UIElement();

    ikdtree::Box<2> GetBoundingBox() const ;
    graphic::Node* node() const { return node_; }

    void set_owner(Menu* owner) { owner_ = owner; }

    void Interact() const { if(function_) function_(this); }

  private:
    Menu* owner_;
    std::string name_;
    UICallback function_;
    Vector2D top_left_;
    graphic::Node* node_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_UIELEMENT_H_ */
