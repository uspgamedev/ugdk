#ifndef UGDK_UI_DRAWABLE_H_
#define UGDK_UI_DRAWABLE_H_

#include <ugdk/graphic/drawable.h>

#include <ugdk/graphic.h>
#include <memory>

namespace ugdk {
namespace ui {

class Drawable : public ugdk::graphic::Drawable {
  public:
    Drawable(const std::shared_ptr<ugdk::graphic::Primitive>& primitive);
    ~Drawable();
    
    void Draw(ugdk::graphic::Canvas&) const override;
    const ugdk::math::Vector2D& size() const override;

  protected:
    void updateSize();

    std::shared_ptr<ugdk::graphic::Primitive> primitive_;
    ugdk::math::Vector2D size_;
};

} // namespace ui
} // namespace ugdk

#endif // UGDK_UI_DRAWABLE_H_
