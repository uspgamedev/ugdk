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

    class Glyph {
      public:
        const math::Vector2D& vertex_top_left() const { return vertex_top_left_; }
        const math::Vector2D& vertex_bottom_right() const { return vertex_bottom_right_; }
        const math::Vector2D& texture_top_left() const { return texture_top_left_; }
        const math::Vector2D& texture_bottom_right() const { return texture_bottom_right_; }

        bool operator==(const Glyph& rhs) const {
            return this->string_iterator_ == rhs.string_iterator_;
        }
        bool operator!=(const Glyph& rhs) const { return ! this->operator==(rhs); }


        Glyph& operator++();

      private:
        std::wstring::const_iterator string_iterator_, end_;
        freetypeglxx::TextureFont* freetype_font_;
        math::Vector2D pen_;
        math::Vector2D vertex_top_left_, vertex_bottom_right_;
        math::Vector2D texture_top_left_, texture_bottom_right_;

        Glyph(freetypeglxx::TextureFont* font, const std::wstring& string, 
            const math::Vector2D& initial_pen);

        void updatePositions(wchar_t new_char, wchar_t previous);

        friend class Font;
    };

    Glyph Write(const std::wstring& string, const math::Vector2D& start = math::Vector2D()) {
        return Glyph(freetype_font_, string, start);
    }

  private:
    freetypeglxx::TextureAtlas* atlas_;
    freetypeglxx::TextureFont* freetype_font_;
    double size_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
