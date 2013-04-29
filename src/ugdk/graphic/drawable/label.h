#ifndef UGDK_GRAPHIC_DRAWABLE_LABEL_H_
#define UGDK_GRAPHIC_DRAWABLE_LABEL_H_

#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>

namespace ugdk {
namespace graphic {

class Label : public Drawable {
  public:
    Label(const std::wstring& message, Font* font);
    ~Label();

    /// This operation is slow.
    void ChangeMessage(const std::wstring& message);

    void Update(double) {}
    void Draw(const Geometry&, const VisualEffect&) const;

    const ugdk::math::Vector2D& size() const;

  private:
    Font* font_;
    opengl::VertexBuffer* vertex_buffer_;
    opengl::VertexBuffer* texture_buffer_;

    ugdk::math::Vector2D size_;
    std::wstring message_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
