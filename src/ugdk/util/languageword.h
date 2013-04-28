#ifndef UGDK_UTIL_LANGUAGEWORD_H_
#define UGDK_UTIL_LANGUAGEWORD_H_

#include <string>
#include <ugdk/graphic.h>

namespace ugdk {

class LanguageWord {
  public:
    virtual ~LanguageWord() {}
    virtual graphic::TextBox* GenerateText() const = 0;
    const std::string& font() const { return font_; }
  protected:
    LanguageWord(const std::string& font) : font_(font) {}
    std::string font_;
};

}  // namespace ugdk

#endif /* UGDK_UTIL_LANGUAGEWORD_H_ */
