#ifndef HORUSEYE_FRAMEWORK_LIGHT_H_
#define HORUSEYE_FRAMEWORK_LIGHT_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/structure/types.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/vertexdata.h>

namespace ugdk {
namespace graphic {

class Light {
  public:
    Light();
	~Light();

    const ugdk::math::Vector2D& dimension() const { return dimension_; }
    void set_dimension(const ugdk::math::Vector2D& dimension);

    const Color& color() const { return color_; }
    void set_color(const Color& color) { color_ = color; }

    void Draw(Canvas&);

  private:
	ugdk::math::Vector2D dimension_;
    Color color_;
    VertexData data_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
