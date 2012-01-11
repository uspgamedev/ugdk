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

void Text::Draw(float dt) {
    // DOES NOTHING WITH TIME! TODO?
    Font::IdentType ident = font_->ident();

    glEnable(GL_TEXTURE_2D);
    glListBase(font_->id());

    /*Frame bounds = VIDEO_MANAGER()->virtual_bounds();
    if (position.x > bounds.right() || position.y > bounds.bottom() ||
        position.x + width_ < bounds.left() || position.y + height_ < bounds.top() ) {
        return true;
    }*/
    
    static float FANCY_COLORS[3][3] = {
        {1.000000f, 1.000000f, 1.000000f}, // 255, 255, 255
        {0.831372f, 0.666666f, 0.000000f}, // 212, 170,   0
        {0.333333f, 0.266666f, 0.000000f}};//  85,  68,   0


    int fancy_line_number = 0;
    if (font_->IsFancy()) fancy_line_number = 2;

    glPushMatrix();
    for(; fancy_line_number >= 0; fancy_line_number--) {
        glTranslatef(-1.0f, -1.0f, 0);
        glPushMatrix();
        glColor3fv(FANCY_COLORS[fancy_line_number]);
        for(size_t i = 0; i < message_.size(); ++i) {
            if(message_[i].length() > 0) {
                glPushMatrix();
                switch(ident) {
                    case Font::CENTER:
                        glTranslatef((this->width_ - this->line_width_[i])*0.5f, 0.0f, 0.0f);
                        break;
                    case Font::RIGHT:
                        glTranslatef((this->width_ - this->line_width_[i])*1.0f, 0.0f, 0.0f);
                        break;
                    default:
                        break;
                }
                #ifdef WIN32
                    glCallLists(message_[i].length(), GL_SHORT, (GLshort *) message_[i].c_str());
                #else
                    glCallLists(message_[i].length(), GL_INT, (GLint *) message_[i].c_str());
                #endif
                glPopMatrix();
            }
            glTranslatef( 0, (GLfloat)(line_height_), 0);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

}  // namespace framework
