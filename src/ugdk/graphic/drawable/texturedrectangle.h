#ifndef UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_
#define UGDK_GRAPHIC_DRAWABLE_TEXTUREDRECTANGLE_H_

#include <ugdk/base/types.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable/shape.h>

namespace ugdk {

class TexturedRectangle : public Shape {
  public:
    TexturedRectangle(Texture* texture);
    TexturedRectangle(Texture* texture, const Vector2D& size);
    ~TexturedRectangle();

    void Draw(float dt);

    const Vector2D& size() const { return size_; }
    void set_size(const Vector2D& size) { size_ = size; }

  private:
    Vector2D size_;
    Texture* texture_;
};

}  // namespace ugdk

#endif
