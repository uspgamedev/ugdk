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
	Text(std::wstring message, Font* font);
	Text(std::vector<std::wstring> message, Font* font);
	~Text() {}

    void Draw();

	virtual const Vector2D& size() const;
	int width();
	int height();

  private:
    Font* font_;
	int width_, height_, line_height_;
	std::vector<std::wstring> message_;
	std::vector<int> line_width_;
};

}  // namespace framework

#endif
