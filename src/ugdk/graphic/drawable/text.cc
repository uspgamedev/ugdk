#include <SDL/SDL_opengl.h>

#include <ugdk/graphic/drawable/text.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/math/frame.h>
#include <ugdk/graphic/font.h>

namespace ugdk {

Text::Text(std::wstring message, Font *font) : font_(font) {
	size_t i;
	message_.push_back(message);
	width_ = 0;
	line_height_ = height_ = 
		(message.length() == 0)
		? (int)(font_->GetLetterSize(L'\n').y)
		: (int)(font_->GetLetterSize(message[0]).y);
	for(i = 0; i < message.length(); i++)
		width_ += (int)(font_->GetLetterSize(message[i]).x);
}

Text::Text(std::vector<std::wstring> message, Font *font) : font_(font) {
	size_t i;
	int width, size;
	unsigned int j;
	width_ = 0;
	height_ = 0;
	line_height_ = 0;
	for(j = 0; j < message.size(); j++) {
		width = 0;
		size = 0;
		message_.push_back(message[j]);
		height_ += size =
			(message[j].length() == 0 || message[j][0] == L'\0')
			? (int)(font_->GetLetterSize(L'\n').y)
			: (int)(font_->GetLetterSize(message[j][0]).y);
		for(i = 0; i < message[j].length(); i++)
			width += (int)(font_->GetLetterSize(message[j][i]).x);
		line_width_.push_back(width);
		width_ = std::max(width, width_);
		line_height_ = std::max(size, line_height_);
	}
}

const Vector2D& Text::size() const {
	return Vector2D((float)(width_), (float)(height_));
}

int Text::width() {
	return width_;
}

int Text::height() {
	return height_;
}

//bool Text::DrawTo(const Vector2D& position, int frame_number, uint8 mirror,  const Color& color, float alpha, const Vector2D& draw_size) {
void Text::Draw() {
	int fancy_line_number = 0, current_height;
	glPushMatrix();
	Font::IdentType ident = font_->ident();

	glListBase(font_->id());
	glColor3ub(255, 255, 255);
	/*Frame bounds = VIDEO_MANAGER()->virtual_bounds();
	if (position.x > bounds.right() || position.y > bounds.bottom() ||
        position.x + width_ < bounds.left() || position.y + height_ < bounds.top() ) {
        return true;
    }*/
	if (font_->IsFancy())
		fancy_line_number = 2;
	for(; fancy_line_number >= 0; fancy_line_number--) {
		glTranslatef(-1.0f, -1.0f, 0);
		glPushMatrix();
		current_height = 0;
		if (fancy_line_number == 2)
			glColor3ub(85, 68, 0);
		else if (fancy_line_number == 1)
			glColor3ub(212, 170, 0);
		else if (fancy_line_number == 0)
			glColor3ub(255, 255, 255);
		for(size_t i = 0; i < message_.size(); ++i) {
			if(message_[i].length() > 0 /*&&
				(position.y + current_height < bounds.bottom() && 
				position.y + current_height + line_height_ > bounds.top())*/) {
				glPushMatrix();
				switch(ident) {
					case Font::CENTER:
						glTranslatef((GLfloat)(this->width_ - this->line_width_[i])/2.0f,0,0);
						break;
					case Font::RIGHT:
						glTranslatef((GLfloat)(this->width_ - this->line_width_[i]),0,0);
						break;
					default:
						break;
				}
#ifdef WIN32
				glCallLists(message_[i].length(), GL_SHORT, (GLshort *) message_[i].c_str());
#else
				glCallLists(message_[i].length(), GL_INT, (GLshort *) message_[i].c_str());
#endif
				glPopMatrix();
			}
			glTranslatef( 0, (GLfloat)(line_height_), 0);
			current_height += line_height_;
		}
		glPopMatrix();
	}
	glPopMatrix();
}

}  // namespace framework
