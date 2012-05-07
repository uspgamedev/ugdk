#ifndef UGDK_UIELEMENT_H_
#define UGDK_UIELEMENT_H_

#include <functional>
#include <string>

#include <ugdk/util.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/intervalkdtree.h>

class UIElement {
  public:
    typedef std::tr1::function<void (UIElement *)> UICallback;
    
    UIElement(ugdk::Vector2D top_left, ugdk::Vector2D bottom_right, UICallback function)
    : top_left_(top_left), 
      bottom_right_(bottom_right),
      function_(function);
    
    virtual ~UIElement();

    ugdk::ikdtree::Box<2> GetBoundingBox() const { return ugdk::ikdtree::Box<2>(top_left_.val, bottom_right_.val); }
    
    ugdk::graphic::Node* node() const { return node_; }

    void Interact() const { function_(); }

  private:
    std::string name_;
    UICallback function_;
    ugdk::Vector2D top_left_;
    ugdk::Vector2D bottom_right_;
    ugdk::graphic::Node* node_;
};

#endif /* UGDK_UIELEMENT_H_ */
