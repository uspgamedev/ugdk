#ifndef UGDK_TEXT_LANGUAGEWORD_H_
#define UGDK_TEXT_LANGUAGEWORD_H_

#include <string>

namespace ugdk {
namespace text {

class LanguageWord {
  public:
    LanguageWord(const std::string& utf8_text, const std::string& font)
        : utf8_text_(utf8_text), font_(font) {}
    ~LanguageWord() {}

    const std::string& text() const { return utf8_text_; }
    const std::string& font() const { return font_; }

    //ui::Drawable* CreateLabel() const;

  protected:
    std::string utf8_text_;
    std::string font_;
};

} // namespace text
} // namespace ugdk

#endif // UGDK_TEXT_LANGUAGEWORD_H_
