#include "languagemanager.h"

#include <ugdk/util/language.h>
#include <ugdk/debug/log.h>

namespace ugdk {

bool LanguageManager::RegisterLanguage(const std::string& code, const std::string& language_file) {
    // validate the code here
    if(false) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "LanguageManager::RegisterLanguage - Unknown error");
        return false;
    }

    registered_languages_[code] = language_file;
    return true;
}

bool LanguageManager::Setup(const std::string& active_language) {
    if(registered_languages_.find(default_language_) == registered_languages_.end()) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "LanguageManager::Setup - Default language '", default_language_, "' is not registered");
        return false;
    }

    if(registered_languages_.find(active_language) == registered_languages_.end()) {
        debug::Log(debug::LogLevel::ERROR, "UGDK",
                   "LanguageManager::Setup - Active language '", active_language, "' is not registered");
        return false;
    }

    // We'll simply ignore the default language for now... TODO!
    Language lang;
    return lang.Load(registered_languages_[active_language]);
}

} //  namespace ugdk
