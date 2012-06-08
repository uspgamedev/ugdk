#ifndef UGDK_BUTTON_H_
#define UGDK_BUTTON_H_

#include <ugdk/ui/uielement.h>

class UIElement: public UIElement {
  public:
    Button(ugdk::Vector2D top_left, ugdk::Vector2D bottom_right, , ugdk::graphic::Drawable* drawable)
    : UIElement(top_left, bottom_right), 
      function_(function) { node_->set_drawable(drawable); }
    ~Button();

    void Interact() { function_(); }

  private:
};

#endif /* UGDK_BUTTON_H_ */
