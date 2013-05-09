#include <ugdk/graphic/drawable/textbox.h>

#include <sstream>
#include <freetype-gl++/texture-font.hpp>

#include <ugdk/graphic/font.h>
#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/drawable/label.h>

namespace ugdk {
namespace graphic {

using std::vector;
using std::list;
using std::wstring;

TextBox::TextBox(const std::wstring& message, double width, Font* font)
    : font_(font), width_(width), ident_style_(LEFT) {
        ChangeMessage(message);
}

TextBox::~TextBox() {
    for(vector<Label*>::iterator it = labels_.begin(); it != labels_.end(); ++it)
        delete *it;
    labels_.clear();
}

void TextBox::ChangeMessage(const std::wstring& message) {
    for(vector<Label*>::iterator it = labels_.begin(); it != labels_.end(); ++it)
        delete *it;
    labels_.clear();

    size_ = math::Vector2D(width_, 0.0);
    std::list<std::wstring> split_string;
    splitString(message, split_string);
    labels_.reserve(split_string.size());

    for(list<wstring>::const_iterator it = split_string.begin(); it != split_string.end(); ++it) {
        labels_.push_back(new Label(*it, font_));
        size_.y += labels_.back()->size().y;
    }
}

void TextBox::Draw(const Geometry& geo, const VisualEffect& eff) const {
    
    if(draw_setup_function_) draw_setup_function_(this, geo, eff);

    double off_y = 0.0;
    for(vector<Label*>::const_iterator it = labels_.begin(); it != labels_.end(); ++it) {
        double off_x = 0.0;
        if(ident_style_ == CENTER)
            off_x = (width_ - (*it)->width()) * 0.5;
        else if(ident_style_ == RIGHT)
            off_x = (width_ - (*it)->width()) * 1.0;
        (*it)->Draw(geo * Geometry(math::Vector2D(off_x, off_y) - hotspot_), eff);
        off_y += (*it)->size().y;
    }
}
    
void TextBox::splitString(const wstring& message, list<wstring>& strings) {
    list<wstring> line_split;
    {
        std::wstringstream ss(message);
        wstring item;
        while (std::getline(ss, item, L'\n')) {
            line_split.push_back(item);
        }
    }
    for(list<wstring>::iterator it = line_split.begin(); it != line_split.end(); ++it) {
        size_t guaranteed = it->find_first_of(L' ');
        do {
            size_t search = it->find_first_of(L' ', guaranteed + 1);
            if(calculateWidth(*it, search) > width_)
                break;
            guaranteed = search;
        } while(guaranteed != wstring::npos);
        
        if(guaranteed != wstring::npos) {
            list<wstring>::iterator next = it;
            line_split.insert(++next, it->substr(guaranteed + 1));
            *it = it->substr(0, guaranteed);
        }
        strings.push_back(*it);
    }
}

double TextBox::calculateWidth(const std::wstring& str, size_t max) {
    double width = 0.0;
    max = std::min(max, str.size());
    for(size_t i = 0; i < max; ++i ) {
        freetypeglxx::TextureGlyph* glyph = font_->freetype_font()->GetGlyph(str[i]);
        if(!glyph) continue;

        double kerning = 0;
        if(i > 0)
            kerning = glyph->GetKerning(str[i-1]);

        width += kerning;
        width += glyph->advance_x();
    }
    return width;
}

}  // namespace graphic
}  // namespace ugdk
