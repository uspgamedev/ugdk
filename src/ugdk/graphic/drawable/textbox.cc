#include <ugdk/graphic/drawable/textbox.h>

#include <sstream>
#include <iterator>
#include <algorithm>
#include <freetype-gl++/texture-font.hpp>

#include <ugdk/graphic/font.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/drawable/label.h>

namespace ugdk {
namespace graphic {

using std::vector;
using std::list;

TextBox::TextBox(const std::string& utf8_message, double width, Font* font)
    : font_(font), width_(width), ident_style_(LEFT) {
        ChangeMessage(utf8_message);
}

TextBox::~TextBox() {}

void TextBox::ChangeMessage(const std::string& utf8_message) {
    labels_.clear();

    UCS4Vector ucs4_message = utf8_to_ucs4(utf8_message);
    size_ = math::Vector2D(width_, 0.0);

    std::list<UCS4Vector> split_ucs4_message;
    splitString(ucs4_message, split_ucs4_message);

    labels_.reserve(split_ucs4_message.size());

    for(const auto& ucs4_line : split_ucs4_message) {
        labels_.emplace_back(new Label(ucs4_line, font_));
        size_.y += labels_.back()->size().y;
    }
}

void TextBox::Draw(const Geometry& geo, const VisualEffect& eff) const {
    
    if(draw_setup_function_) draw_setup_function_(this, geo, eff);

    double off_y = 0.0;
    for(const auto& label : labels_) {
        double off_x = 0.0;
        if(ident_style_ == CENTER)
            off_x = (width_ - label->width()) * 0.5;
        else if(ident_style_ == RIGHT)
            off_x = (width_ - label->width()) * 1.0;
        label->Draw(geo * Geometry(math::Vector2D(off_x, off_y) - hotspot_), eff);
        off_y += label->size().y;
    }
}
    
void TextBox::splitString(const UCS4Vector& ucs4_message, std::list<UCS4Vector>& split_ucs4_message) {
    list<UCS4Vector> line_split;
    {
        auto start = ucs4_message.begin();
        for(auto it = ucs4_message.begin(); it != ucs4_message.end(); ++it) {
            if(*it == '\n') {
                line_split.emplace_back(start, it);
                start = it + 1;
            }
        }
        line_split.emplace_back(start, ucs4_message.end());
    }
    for(list<UCS4Vector>::iterator it = line_split.begin(); it != line_split.end(); ++it) {
        bool skip = true;

        auto guaranteed = std::find(it->begin(), it->end(), ' ');
        while(guaranteed != it->end()) {
            auto search = std::find(guaranteed + 1, it->end(), ' ');
            if(calculateWidth(it->begin(), search) > width_)
                break;
            guaranteed = search;
        } 

        // If the first word is too big, split it anyway.
        while(guaranteed != it->begin() && calculateWidth(it->begin(), guaranteed) > width_) {
            skip = false;
            guaranteed--;
        }
        
        if(guaranteed != it->end()) {
            auto next = it;
            line_split.emplace(++next, guaranteed + (skip ? 1 : 0), it->end());
        }
        split_ucs4_message.emplace_back(it->begin(), guaranteed);
    }
}

double TextBox::calculateWidth(UCS4Vector::const_iterator start, UCS4Vector::const_iterator end) {
    double width = 0.0;

    bool has_previous = false;
    UCS4Vector::const_iterator previous;
    for(auto step = start; step != end; previous = step++) {
        freetypeglxx::TextureGlyph* glyph = font_->freetype_font()->GetGlyph(*step);
        if(!glyph) continue;

        double kerning = 0;
        if(has_previous)
            kerning = glyph->GetKerning(*previous);
        else
            has_previous = true;

        width += kerning;
        width += glyph->advance_x();
    }
    return width;
}

}  // namespace graphic
}  // namespace ugdk
