#ifndef PYRAMIDWORKS_UI_BUTTON_H_
#define PYRAMIDWORKS_UI_BUTTON_H_

#include <pyramidworks/ui/uielement.h>
#include <ugdk/math/vector2D.h>

#include <functional>

namespace pyramidworks {
namespace ui {

typedef std::function<void (const Button *)> ButtonCallback;
class Button : public UIElement {
  public:
    Button(const ugdk::math::Vector2D& top_left, ugdk::graphic::Drawable* drawable, const ButtonCallback& function);
    ~Button();

    void Interact() const { if(function_) function_(this); }
    ugdk::structure::Box<2> GetBoundingBox() const;

  private:
    ButtonCallback function_;
    ugdk::math::Vector2D top_left_;
};

} // namespace ui
} // namespace pyramidworks

#endif // PYRAMIDWORKS_UI_BUTTON_H_
