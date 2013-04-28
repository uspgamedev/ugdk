#ifndef UGDK_UTIL_LANGUAGEWORD_H_
#define UGDK_UTIL_LANGUAGEWORD_H_

#include <string>
#include <ugdk/graphic.h>

namespace ugdk {

class LanguageWord {
  public:
    LanguageWord(const std::wstring& text, const std::string& font) 
        : text_(text), font_(font) {}
    ~LanguageWord() {}

    const std::wstring& text() const { return text_; }
    const std::string& font() const { return font_; }

    graphic::Drawable* CreateLabel() const;

  protected:
    std::wstring text_;
    std::string font_;
};

}  // namespace ugdk

#endif /* UGDK_UTIL_LANGUAGEWORD_H_ */
