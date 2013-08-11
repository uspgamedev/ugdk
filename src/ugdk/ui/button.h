#ifndef UGDK_UI_BUTTON_H_
#define UGDK_UI_BUTTON_H_

#include <functional>

#include <ugdk/ui/uielement.h>
#include <ugdk/math/vector2D.h>

namespace ugdk {
namespace ui {

typedef std::function<void (const Button *)> ButtonCallback;
class Button : public UIElement {
  public:
    Button(const ugdk::math::Vector2D& top_left, graphic::Drawable* drawable, const ButtonCallback& function);
    ~Button();

    void Interact() const { if(function_) function_(this); }
    structure::Box<2> GetBoundingBox() const;

  private:
    ButtonCallback function_;
    ugdk::math::Vector2D top_left_;
};

} // namespace ui
} // namespace ugdk

#endif /* UGDK_BUTTON_H_ */
