#include <ugdk/graphic/font.h>

#include <freetype-gl++/texture-atlas.hpp>
#include <freetype-gl++/texture-font.hpp>

#include <ugdk/base/engine.h>
#include <ugdk/util/pathmanager.h>

namespace ugdk {
namespace graphic {

using std::string;
using std::wstring;
using freetypeglxx::TextureFont;
using math::Vector2D;

Font::Font(const string& path, double size, int num_glyphs) 
    : atlas_(new freetypeglxx::TextureAtlas(512, 512, 1)),
      size_(size) {
    
    std::string fullpath = PATH_MANAGER()->ResolvePath(path);
    freetype_font_ = new TextureFont(atlas_, fullpath, size);
}

Font::~Font() {
    delete freetype_font_;
    delete atlas_;
}

double Font::height() const {
    return static_cast<double>(freetype_font_->height());
}
    
void Font::HintString(const wstring& string) {
    if(freetype_font_->LoadGlyphs(string.c_str()) > 0) {
        // TODO: Could not generate all glyphs!
    }
}

Font::Glyph::Glyph(TextureFont* font, const wstring& string, const Vector2D& initial_pen)
    : string_iterator_(string.begin()), end_(string.end()), freetype_font_(font), pen_(initial_pen) {
    if(string_iterator_ != end_)
        updatePositions(*string_iterator_, -1);
}

Font::Glyph& Font::Glyph::operator++() {
    wchar_t previous = *string_iterator_;
    if(++string_iterator_ != end_)
        updatePositions(*string_iterator_, previous);
    return *this;
}
        
void Font::Glyph::updatePositions(wchar_t new_char, wchar_t previous) {
    freetypeglxx::TextureGlyph* glyph = freetype_font_->GetGlyph(new_char);
    double kerning = (previous != -1) ? glyph->GetKerning(previous) : 0.0;
    pen_.x += kerning;

    vertex_top_left_.x = pen_.x + glyph->offset_x();
    vertex_top_left_.y = glyph->offset_y();

    vertex_bottom_right_.x = vertex_top_left_.x + glyph->width();
    vertex_bottom_right_.y = vertex_top_left_.y - glyph->height();

    vertex_top_left_.y     = pen_.y + freetype_font_->height() - vertex_top_left_.y;
    vertex_bottom_right_.y = pen_.y + freetype_font_->height() - vertex_bottom_right_.y;

    texture_top_left_     = math::Vector2D(glyph->s0(), glyph->t0());
    texture_bottom_right_ = math::Vector2D(glyph->s1(), glyph->t1());

    pen_.x += glyph->advance_x();
}

} // namespace graphic
} // namespace ugdk
