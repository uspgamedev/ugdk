#include <ugdk/config/config.h>
#include "SDL_opengl.h"

#include <ugdk/graphic/drawable/text.h>

#include <ugdk/base/engine.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/videomanager.h>
#include <ugdk/math/frame.h>
#include <ugdk/graphic/font.h>

namespace ugdk {
namespace graphic {

using ugdk::Color;

static Vector2D GetStringSize(const std::wstring& string, Font* font) {
    double width = 0, height = font->GetLetterSize(L'\n').y;
    for(size_t i = 0; i < string.length(); i++) {
        const Vector2D letter_size = font->GetLetterSize(string[i]);
        width += letter_size.x;
        height = std::max(height, letter_size.y);
    }
    return Vector2D(width, height);
}

Text::Text(const std::wstring& message, Font *font) : font_(font) {
    this->SetMessage(message);
}

Text::Text(const std::vector<std::wstring>& message, Font *font) : font_(font) {
    this->SetMessage(message);
}

void Text::SetMessage(const std::wstring& message) {
    message_.clear();
    message_.push_back(message);
    size_ = GetStringSize(message, font_);
    line_height_ = size_.y;
    line_width_.clear();
    line_width_.push_back(size_.x);
}

void Text::SetMessage(const std::vector<std::wstring>& message) {
    std::vector<std::wstring>::const_iterator it;
    for(it = message.begin(); it != message.end(); ++it) {
        Vector2D line_size = GetStringSize(*it, font_);
        line_height_ = std::max(line_size.y, line_height_);

        size_.x = std::max(size_.x, line_size.x);
        size_.y += line_size.y;

        line_width_.push_back(line_size.x);
    }
    message_.clear();
    message_ = message;
}

const Vector2D& Text::size() const {
    return size_;
}

void Text::Update(double dt) {}

void Text::Draw() const {
    const Modifier& mod = VIDEO_MANAGER()->CurrentModifier();
    if(!mod.visible()) return;
    
    static Color FANCY_COLORS[3] = {
        Color(1.000000, 1.000000, 1.000000), // 255, 255, 255
        Color(0.831372, 0.666666, 0.000000), // 212, 170,   0
        Color(0.333333, 0.266666, 0.000000)};//  85,  68,   0
        
    Font::IdentType ident = font_->ident();

    glEnable(GL_TEXTURE_2D);
    glListBase(font_->id());

    glPushMatrix();
    
    // TODO: combine the hotspot and mirror matrices.

    // Hotspot
    glTranslated(-hotspot_.x, -hotspot_.y, 0.0);

    // horizontal flip
    if(mod.mirror() & MIRROR_HFLIP) {
        glTranslated(size_.x, 0.0, 0.0);
        glScaled(-1.0, 1.0, 1.0);
    }

    // vertical flip
    if(mod.mirror() & MIRROR_VFLIP) {
        glTranslated(0.0, size_.y, 0.0);
        glScaled(1.0, -1.0, 1.0);
    }
    
    int fancy_line_number = 0;
    if (font_->IsFancy()) fancy_line_number = 2;
    for(; fancy_line_number >= 0; fancy_line_number--) {
        glTranslated(-1.0, -1.0, 0);
        glPushMatrix();
        Color final_color = mod.color() * FANCY_COLORS[fancy_line_number];
        glColor4dv(final_color.val);
        for(size_t i = 0; i < message_.size(); ++i) {
            if(i != 0) glTranslated( 0.0, line_height_, 0.0);
            if(message_[i].length() > 0) {
                glPushMatrix();
                switch(ident) {
                    case Font::CENTER:
                        glTranslated((size_.x - this->line_width_[i])*0.5, 0.0, 0.0);
                        break;
                    case Font::RIGHT:
                        glTranslated((size_.x - this->line_width_[i])*1.0, 0.0, 0.0);
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
        }
        glPopMatrix();
    }
    glPopMatrix();
}

}  // namespace graphic
}  // namespace ugdk
