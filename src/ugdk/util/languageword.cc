#include <ugdk/util/languageword.h>

#include <ugdk/system/engine.h>
#include <ugdk/text/module.h>
#include <ugdk/text/label.h>

namespace ugdk {

graphic::Drawable* LanguageWord::CreateLabel() const {
    return new text::Label(utf8_text_, text::manager()->GetFont(font_));
}

} // namespace ugdk