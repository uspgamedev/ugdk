#ifndef HORUSEYE_FRAMEWORK_LIGHT_H_
#define HORUSEYE_FRAMEWORK_LIGHT_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic.h>

namespace ugdk {

class Light {
  public:
    Light();
	~Light() {}

    const Vector2D& dimension() const { return dimension_; }
	void set_dimension(const Vector2D& dimension) { dimension_ = dimension; }

    const Color& color() const { return color_; }
    void set_color(const Color& color) { color_ = color; }

	void Draw();

  private:
	Vector2D dimension_;
    Color color_;
};

}  // namespace framework

#endif
