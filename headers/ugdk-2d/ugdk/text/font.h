#ifndef UGDK_TEXT_FONT_H_
#define UGDK_TEXT_FONT_H_

#include <ugdk/text.h>
#include <ugdk/math/vector2D.h>

#include <memory>
#include <string>

struct texture_atlas_t;
struct texture_font_t;

namespace ugdk {
namespace text {

class Font {
  public:
    Font(const std::string& path, double size);
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

}  // namespace text
}  // namespace ugdk

#endif // UGDK_TEXT_FONT_H_
