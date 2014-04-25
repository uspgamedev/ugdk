#ifndef UGDK_GRAPHIC_DRAWABLE_LABEL_H_
#define UGDK_GRAPHIC_DRAWABLE_LABEL_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/vertexdata.h>
#include <ugdk/util/utf8.h>
#include <ugdk/structure/types.h>

#include <string>
#include <cstddef>
#include <vector>
#include <memory>

namespace ugdk {
namespace graphic {

class Label : public Drawable {
  public:
    Label(const std::string& utf8_message, Font* font);
    Label(const std::u32string& ucs4_message, Font* font);
    ~Label();

    /// This operation is slow.
    void ChangeMessage(const std::string& utf8_message);
    void ChangeMessage(const std::u32string& ucs4_message);

    void Draw(Canvas&) const override;
    const ugdk::math::Vector2D& size() const override;

  private:
    Font* font_;
    std::unique_ptr<VertexData> buffer_;
    std::vector<int> first_vector_, size_vector_;

    ugdk::math::Vector2D size_;
    std::size_t num_characters_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
