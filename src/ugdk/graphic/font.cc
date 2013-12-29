#include <ugdk/graphic/font.h>

#include <ugdk/system/engine.h>
#include <texture-atlas.h>
#include <texture-font.h>

namespace ugdk {
namespace graphic {

using std::string;
using std::wstring;
using math::Vector2D;

Font::Font(const string& path, double size, int num_glyphs) 
    : size_(size) 
    {
    
    std::string fullpath = ugdk::system::ResolvePath(path);

    atlas_ = texture_atlas_new(512, 512, 1);

    freetype_font_ = texture_font_new_from_file(
                            atlas_,
                            static_cast<float>(size),
                            fullpath.c_str());
}

Font::~Font() {
    texture_atlas_delete(atlas_);
    texture_font_delete(freetype_font_);
}

double Font::height() const {
    return static_cast<double>(freetype_font_->height);
}
    
void Font::HintString(const wstring& string) {
    if(texture_font_load_glyphs(freetype_font_, string.c_str()) > 0) {
        // TODO: Could not generate all glyphs!
    }
}

} // namespace graphic
} // namespace ugdk
