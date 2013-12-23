#ifndef UGDK_GRAPHIC_FONT_H_
#define UGDK_GRAPHIC_FONT_H_

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#include <memory>
#include <string>

namespace freetypeglxx {
    class TextureAtlas;
    class TextureFont;
}

namespace ugdk {
namespace graphic {

class Font {
  public:
    Font(const std::string& path, double size, int num_glyphs = 128);
    ~Font();

    double size() const { return size_; }
    double height() const;

    /// Generates all glyphs in the given string and caches then for later use.
    void HintString(const std::wstring&);

    freetypeglxx::TextureFont* freetype_font() {
#ifndef UGDK_USING_GLES
        return freetype_font_.get();
#else
        return nullptr;
#endif
    }

  private:
    double size_;
#ifndef UGDK_USING_GLES
    std::unique_ptr<freetypeglxx::TextureAtlas> atlas_;
    std::unique_ptr<freetypeglxx::TextureFont> freetype_font_;
#endif
};

}  // namespace graphic
}  // namespace ugdk

#endif
