#include <cstdio>
#include "languagemanager.h"
#include <ugdk/util/language.h>

namespace ugdk {

bool LanguageManager::RegisterLanguage(const std::string& code, const std::string& language_file) {
    // validate the code here
    if(false) {
        fprintf(stderr, "LanguageManager::RegisterLanguage Error - Unknown error.\n");
        return false;
    }

    registered_languages_[code] = language_file;
    return true;
}

bool LanguageManager::Setup(const std::string& active_language) {
    if(registered_languages_.find(default_language_) == registered_languages_.end()) {
        fprintf(stderr, "LanguageManager::Setup Error - Default language '%s' is not registered.\n", default_language_.c_str());
        return false;
    }

    if(registered_languages_.find(active_language) == registered_languages_.end()) {
        fprintf(stderr, "LanguageManager::Setup Error - Active language '%s' is not registered.\n", active_language.c_str());
        return false;
    }

    // We'll simply ignore the default language for now... TODO!
    Language lang;
    return lang.Load(registered_languages_[active_language]);
}

} //  namespace ugdk
