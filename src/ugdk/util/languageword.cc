#include <ugdk/util/languageword.h>

#include <ugdk/system/engine.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/drawable/label.h>

namespace ugdk {

graphic::Drawable* LanguageWord::CreateLabel() const {
    return new graphic::Label(utf8_text_, TEXT_MANAGER()->GetFont(font_));
}

} // namespace ugdk