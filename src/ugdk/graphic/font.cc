#include <ugdk/graphic/font.h>

//#include <freetype-gl++/texture-atlas.hpp> FIXME
//#include <freetype-gl++/texture-font.hpp> FIXME

#include <ugdk/system/engine.h>

namespace ugdk {
namespace graphic {

using std::string;
using std::wstring;
//using freetypeglxx::TextureFont; FIXME
using math::Vector2D;

Font::Font(const string& path, double size, int num_glyphs) 
    : atlas_(nullptr), //FIXME new freetypeglxx::TextureAtlas(512, 512, 1)),
      freetype_font_(nullptr),
      size_(size) {
    
    std::string fullpath = ugdk::system::ResolvePath(path);
    //freetype_font_ = new TextureFont(atlas_, fullpath, static_cast<float>(size));
}

Font::~Font() {
    //delete freetype_font_; FIXME
    //delete atlas_;
}

double Font::height() const {
    return 0; //FIXME static_cast<double>(freetype_font_->height());
}
    
void Font::HintString(const wstring& string) {
    //if(freetype_font_->LoadGlyphs(string.c_str()) > 0) { FIXME
        // TODO: Could not generate all glyphs!
    //}
}

} // namespace graphic
} // namespace ugdk
