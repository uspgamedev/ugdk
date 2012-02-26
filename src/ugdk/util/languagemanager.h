#ifndef UGDK_UTIL_LANGUAGEMANAGER_H_
#define UGDK_UTIL_LANGUAGEMANAGER_H_

#include <string>
#include <map>
#include <ugdk/util.h>

namespace ugdk {

#define LANGUAGE_MANAGER() (ugdk::base::Engine::reference()->language_manager())

class LanguageManager {
  public:
    LanguageManager(const std::string& default_language_code) : default_language_(default_language_code) {}
    ~LanguageManager() {}

    const std::string& default_language() const { return default_language_; }

    bool RegisterLanguage(const std::string& code, const std::string& language_file);

    /** @param active_language The language that will be active. */
    bool Setup(const std::string& active_language);

  private:
    std::map<std::string, std::string> registered_languages_;
    std::string default_language_;
};

}  // namespace ugdk

#endif /* UGDK_UTIL_LANGUAGEMANAGER_H_ */
