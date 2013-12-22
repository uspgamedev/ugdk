#include <ugdk/graphic/font.h>

#ifndef UGDK_USING_GLES // TODO: freetype-gl for android
#include <freetype-gl++/texture-atlas.hpp>
#include <freetype-gl++/texture-font.hpp>
#endif

#include <ugdk/system/engine.h>

namespace ugdk {
namespace graphic {

using std::string;
using std::wstring;
using freetypeglxx::TextureFont;
using math::Vector2D;

Font::Font(const string& path, double size, int num_glyphs) 
    : size_(size) 

#ifndef UGDK_USING_GLES
    , atlas_(new freetypeglxx::TextureAtlas(512, 512, 1))
#endif
    {
    
    std::string fullpath = ugdk::system::ResolvePath(path);
#ifndef UGDK_USING_GLES
    freetype_font_.reset(new TextureFont(atlas_.get(), fullpath, static_cast<float>(size)));
#endif
}

Font::~Font() {}

double Font::height() const {
#ifndef UGDK_USING_GLES
    return static_cast<double>(freetype_font_->height());
#else
    return 0;
#endif
}
    
void Font::HintString(const wstring& string) {
#ifndef UGDK_USING_GLES
    if(freetype_font_->LoadGlyphs(string.c_str()) > 0) {
        // TODO: Could not generate all glyphs!
    }
#endif
}

} // namespace graphic
} // namespace ugdk
