#include <ugdk/graphic/font.h>

#ifndef UGDK_USING_GLES // TODO: freetype-gl for android
#include <texture-atlas.h>
#include <texture-font.h>
#endif

#include <ugdk/system/engine.h>

namespace ugdk {
namespace graphic {

using std::string;
using std::wstring;
using math::Vector2D;

Font::Font(const string& path, double size, int num_glyphs) 
    : size_(size) 
    {
    
    std::string fullpath = ugdk::system::ResolvePath(path);

#ifndef UGDK_USING_GLES
    atlas_ = texture_atlas_new(512, 512, 1);

    freetype_font_ = texture_font_new_from_file(
                            atlas_,
                            static_cast<float>(size),
                            fullpath.c_str());
#endif
}

Font::~Font() {
#ifndef UGDK_USING_GLES
    texture_atlas_delete(atlas_);
    texture_font_delete(freetype_font_);
#endif
}

double Font::height() const {
#ifndef UGDK_USING_GLES
    return static_cast<double>(freetype_font_->height);
#else
    return 0.0;
#endif
}
    
void Font::HintString(const wstring& string) {
#ifndef UGDK_USING_GLES
    if(texture_font_load_glyphs(freetype_font_, string.c_str()) > 0) {
        // TODO: Could not generate all glyphs!
    }
#endif
}

} // namespace graphic
} // namespace ugdk
