#ifndef HORUSEYE_FRAMEWORK_TEXT_H_
#define HORUSEYE_FRAMEWORK_TEXT_H_

#include <vector>
#include <string>
#include "vector2D.h"
#include "drawable.h"

namespace ugdk {

class Font;
class Text : public Drawable {
  public:
	Text(std::wstring message, Font* font);
	Text(std::vector<std::wstring> message, Font* font);
	~Text() {}

	bool DrawTo(const Vector2D& position, int frame_number, uint8 mirror, 
		const Color& color, float alpha, const Vector2D& draw_size);

	virtual Vector2D render_size() const;
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
