#ifndef UGDK_UTIL_LANGUAGEWORD_H_
#define UGDK_UTIL_LANGUAGEWORD_H_

#include <string>
#include <ugdk/graphic.h>

namespace ugdk {

class LanguageWord {
  public:
    LanguageWord(const std::string& utf8_text, const std::string& font) 
        : utf8_text_(utf8_text), font_(font) {}
    ~LanguageWord() {}

    const std::string& text() const { return utf8_text_; }
    const std::string& font() const { return font_; }

    graphic::Drawable* CreateLabel() const;

  protected:
    std::string utf8_text_;
    std::string font_;
};

}  // namespace ugdk

#endif /* UGDK_UTIL_LANGUAGEWORD_H_ */
