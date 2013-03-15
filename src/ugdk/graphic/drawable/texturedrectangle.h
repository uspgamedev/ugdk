#ifndef UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_
#define UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_

#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable/shape.h>

namespace ugdk {
namespace graphic {

class TexturedRectangle : public Drawable {
  public:
    TexturedRectangle(Texture* texture);
    TexturedRectangle(Texture* texture, const math::Vector2D& _size);
    ~TexturedRectangle();

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

}  // namespace graphic
}  // namespace ugdk

#endif
