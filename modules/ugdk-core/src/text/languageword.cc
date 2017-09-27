
#include <ugdk/text/languageword.h>

#include <ugdk/system/engine.h>
#include <ugdk/text/module.h>

namespace ugdk {
namespace text {

using std::unique_ptr;
using std::make_unique;

unique_ptr<Label> LanguageWord::CreateLabel() const {
    return std::move(make_unique<Label>(utf8_text_, text::manager().GetFont(font_)));
}

} // namespace text
} // namespace ugdk
