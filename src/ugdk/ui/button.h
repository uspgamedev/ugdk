#ifndef PYRAMIDWORKS_UI_BUTTON_H_
#define PYRAMIDWORKS_UI_BUTTON_H_

#include <ugdk/ui/uielement.h>
#include <ugdk/math/vector2D.h>

#include <functional>
#include <memory>

namespace ugdk {
namespace ui {

typedef std::function<void (const Button *)> ButtonCallback;
class Button : public UIElement {
  public:
    Button(const math::Vector2D& top_left, std::unique_ptr<Drawable>&& drawable, const ButtonCallback& function);
    ~Button();

    void Interact() const { if(function_) function_(this); }
    structure::Box<2> GetBoundingBox() const;

  private:
    ButtonCallback function_;
    math::Vector2D top_left_;
};

} // namespace ui
} // namespace ugdk

#endif // PYRAMIDWORKS_UI_BUTTON_H_
