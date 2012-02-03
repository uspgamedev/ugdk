#ifndef HORUSEYE_FRAMEWORK_TEXT_H_
#define HORUSEYE_FRAMEWORK_TEXT_H_

#include <vector>
#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/drawable.h>

namespace ugdk {

class Font;
class Text : public Drawable {
  public:
	Text(const std::wstring& message, Font* font);
	Text(const std::vector<std::wstring>& message, Font* font);
	~Text() {}

    void Draw(float dt);

	virtual const Vector2D& size() const;

  private:
    Font* font_;
	std::vector<std::wstring> message_;

    Vector2D size_;
    float line_height_;
	std::vector<float> line_width_;
};

}  // namespace ugdk

#endif
