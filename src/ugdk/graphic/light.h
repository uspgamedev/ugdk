#ifndef HORUSEYE_FRAMEWORK_LIGHT_H_
#define HORUSEYE_FRAMEWORK_LIGHT_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/common/types.h>
#include <ugdk/graphic.h>

namespace ugdk {
namespace graphic {

class Light {
  public:
    Light();
	~Light();

    const ugdk::math::Vector2D& dimension() const { return dimension_; }
	void set_dimension(const ugdk::math::Vector2D& dimension) { dimension_ = dimension; }

    const Color& color() const { return color_; }
    void set_color(const Color& color) { color_ = color; }

	void Draw(const Geometry&);

  private:
	ugdk::math::Vector2D dimension_;
    Color color_;
    opengl::VertexBuffer* vertexbuffer_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
