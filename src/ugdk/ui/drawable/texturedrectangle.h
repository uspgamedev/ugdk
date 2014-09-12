#ifndef UGDK_UI_DRAWABLE_TEXTUREDRECTANGLE_H_
#define UGDK_UI_DRAWABLE_TEXTUREDRECTANGLE_H_

#include <ugdk/internal.h>
#include <ugdk/graphic.h>
#include <ugdk/ui/drawable.h>
#include <ugdk/graphic/vertexdata.h>

namespace ugdk {
namespace ui {

class TexturedRectangle : public Drawable {
  public:
    TexturedRectangle(const internal::GLTexture* texture);
    TexturedRectangle(const internal::GLTexture* texture, const math::Vector2D& _size);
    ~TexturedRectangle();

    void Draw(graphic::Canvas& canvas) const override;

    const math::Vector2D& size() const override {
        return size_;
    }

  private:
    math::Vector2D size_;
    graphic::VertexData data_;
    const internal::GLTexture* texture_;
};

}  // namespace ui
}  // namespace ugdk

#endif // UGDK_UI_DRAWABLE_TEXTUREDRECTANGLE_H_
