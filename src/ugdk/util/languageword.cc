#include <ugdk/util/languageword.h>

#include <ugdk/base/engine.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/drawable/label.h>

namespace ugdk {

graphic::Drawable* LanguageWord::CreateLabel() const {
    return new graphic::Label(text_, TEXT_MANAGER()->GetFont(font_));
}

} // namespace ugdk