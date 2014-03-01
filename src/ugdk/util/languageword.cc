#include <ugdk/util/languageword.h>

#include <ugdk/system/engine.h>
#include <ugdk/graphic/text/textmanager.h>
#include <ugdk/graphic/text/label.h>

namespace ugdk {

graphic::Drawable* LanguageWord::CreateLabel() const {
    return new graphic::Label(utf8_text_, TEXT_MANAGER()->GetFont(font_));
}

} // namespace ugdk