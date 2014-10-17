#ifndef UGDK_UI_DRAWABLE_PRIMITIVE_H_
#define UGDK_UI_DRAWABLE_PRIMITIVE_H_

#include <ugdk/ui/drawable.h>

#include <ugdk/graphic.h>
#include <memory>

namespace ugdk {
namespace ui {

class DrawablePrimitive : public Drawable {
  public:
    DrawablePrimitive(const std::shared_ptr<ugdk::graphic::Primitive>& primitive);
    ~DrawablePrimitive();
    
    void Draw(graphic::Canvas&) const override;
    const math::Vector2D& size() const override;

  protected:
    void UpdateSize();

    std::shared_ptr<ugdk::graphic::Primitive> primitive_;
    math::Vector2D size_;
};

} // namespace ui
} // namespace ugdk

#endif // UGDK_UI_DRAWABLE_PRIMITIVE_H_
