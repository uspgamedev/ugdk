#ifndef UGDK_GRAPHIC_DRAWABLE_MULTITEXT_H_
#define UGDK_GRAPHIC_DRAWABLE_MULTITEXT_H_

#include <vector>
#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>

namespace freetypeglxx {
    class TextureFont;
}

namespace ugdk {
namespace graphic {

class MultiText : public Drawable {
  public:
    MultiText(const std::vector<std::wstring>& message, freetypeglxx::TextureFont* font);
    ~MultiText();

    void SetMessage(const std::vector<std::wstring>& message);

    void Update(double dt) {}
    void Draw(const Geometry&, const VisualEffect&) const;

    virtual const ugdk::math::Vector2D& size() const { return size_; }

  private:
    freetypeglxx::TextureFont* font_;
    std::vector<Text*> texts_;

    ugdk::math::Vector2D size_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
