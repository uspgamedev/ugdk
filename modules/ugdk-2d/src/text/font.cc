#include <ugdk/text/font.h>

#include <ugdk/filesystem/module.h>
#include <ugdk/filesystem/file.h>
#include <texture-atlas.h>
#include <texture-font.h>

namespace ugdk {
namespace text {

using std::string;
using std::wstring;
using math::Vector2D;

Font::Font(const string& path, double size) 
    : size_(size)
    , ttf_memory_(nullptr)
{

    auto file = ugdk::filesystem::manager()->OpenFile(path);
    auto file_size = file->size();

    ttf_memory_ = malloc(file_size);
    file->ReadRaw(ttf_memory_, file_size, 1);
    
    atlas_ = texture_atlas_new(512, 512, 1);

    freetype_font_ = texture_font_new_from_memory(
                            atlas_,
                            static_cast<float>(size),
                            ttf_memory_,
                            file_size);
}

Font::~Font() {
    if (ttf_memory_)
        free(ttf_memory_);
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

} // namespace text
} // namespace ugdk
