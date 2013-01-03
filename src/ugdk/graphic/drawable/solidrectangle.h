#ifndef UGDK_GRAPHIC_DRAWABLE_SOLIDRECTANGLE_H_
#define UGDK_GRAPHIC_DRAWABLE_SOLIDRECTANGLE_H_

#include <ugdk/graphic/drawable/shape.h>
#include <ugdk/base/types.h>

namespace ugdk {
namespace graphic {

class SolidRectangle : public Shape {
  public:
    SolidRectangle(const ugdk::math::Vector2D& size);
    ~SolidRectangle();

    void Update(double dt);
    void Draw(const Geometry&, const VisualEffect&) const;

    const ugdk::math::Vector2D& size() const { return size_; }
    void set_size(const ugdk::math::Vector2D& size) { size_ = size; }

    const Color& color() const { return color_; }
    void set_color(const Color& color) { color_ = color; }

  private:
    ugdk::math::Vector2D size_;
    Color color_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
