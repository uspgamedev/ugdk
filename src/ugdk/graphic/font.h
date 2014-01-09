#ifndef UGDK_GRAPHIC_FONT_H_
#define UGDK_GRAPHIC_FONT_H_

#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>

#include <memory>
#include <string>

struct texture_atlas_t;
struct texture_font_t;

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

    texture_font_t* freetype_font() const {
        return freetype_font_;
    }

  private:
    double size_;
    texture_atlas_t* atlas_;
    texture_font_t* freetype_font_;
};

}  // namespace graphic
}  // namespace ugdk

#endif // UGDK_GRAPHIC_FONT_H_
