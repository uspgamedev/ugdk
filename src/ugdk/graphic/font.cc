#include <ugdk/graphic/font.h>

#include <freetype-gl++/texture-atlas.hpp>
#include <freetype-gl++/texture-font.hpp>

#include <ugdk/system/engine.h>

namespace ugdk {
namespace graphic {

using std::string;
using std::wstring;
using freetypeglxx::TextureFont;
using math::Vector2D;

Font::Font(const string& path, double size, int num_glyphs) 
    : atlas_(new freetypeglxx::TextureAtlas(512, 512, 1)),
      size_(size) {
    
    std::string fullpath = ugdk::system::ResolvePath(path);
    freetype_font_ = new TextureFont(atlas_, fullpath, static_cast<float>(size));
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

} // namespace graphic
} // namespace ugdk
