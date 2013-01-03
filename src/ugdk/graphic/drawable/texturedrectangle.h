#ifndef UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_
#define UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_

#include <ugdk/base/types.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable/shape.h>

namespace ugdk {
namespace graphic {

class TexturedRectangle : public Shape {
  public:
    TexturedRectangle(Texture* texture);
    TexturedRectangle(Texture* texture, const ugdk::math::Vector2D& size);
    ~TexturedRectangle();

    void Update(double dt);
    void Draw(const Geometry&) const;

    const ugdk::math::Vector2D& size() const { return size_; }
    void set_size(const ugdk::math::Vector2D& size) { size_ = size; }

  private:
    ugdk::math::Vector2D size_;
    Texture* texture_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
