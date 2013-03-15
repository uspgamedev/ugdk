#ifndef UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_
#define UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable/shape.h>

namespace ugdk {
namespace graphic {

class Rectangle : public Drawable {
  public:
    Rectangle(Texture* texture);
    Rectangle(Texture* texture, const math::Vector2D& _size);
    ~Rectangle();

    void Update(double dt) {}
    void Draw(const Geometry& geometry, const VisualEffect&) const;

    const ugdk::math::Vector2D& size() const {
        return size_;
    }

  private:
    opengl::VertexBuffer* createBuffer();

    ugdk::math::Vector2D size_;
    Texture* texture_;
    opengl::VertexBuffer* vertexbuffer_;
    opengl::VertexBuffer* uvbuffer_;
};
typedef Rectangle TexturedRectangle;

}  // namespace graphic
}  // namespace ugdk

#endif
