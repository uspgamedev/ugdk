#ifndef UGDK_GRAPHIC_FONT_H_
#define UGDK_GRAPHIC_FONT_H_

#include <string>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

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

    freetypeglxx::TextureFont* freetype_font() { return freetype_font_; }

  private:
    freetypeglxx::TextureAtlas* atlas_;
    freetypeglxx::TextureFont* freetype_font_;
    double size_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
