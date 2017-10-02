#ifndef UGDK_TEXT_MANAGER_H_
#define UGDK_TEXT_MANAGER_H_

#include <ugdk/math/vector2D.h>
#include <ugdk/text.h>

#include <unordered_map>
#include <memory>
#include <string>

namespace ugdk {
namespace text {

class Manager {
  public:
    Manager(const std::string& default_language_code);
    ~Manager();

    bool Initialize();
    bool Release();

    TextBox* GetText(        const std::string& text);
    TextBox* GetText(        const std::string& text, const std::string& font, int width = -1);
    TextBox* GetTextFromFile(const std::string& path);
	TextBox* GetTextFromFile(const std::string& path, const std::string& font, int width = -1);

    Font* current_font() const { return current_font_; }
    Font* GetFont(const std::string& name) const;

    Font* AddFont(const std::string& name, const std::string& path, double size);

    //================
    const std::string& default_language() const { return default_language_; }
    const std::string& active_language() const { return active_language_; }

    bool RegisterLanguage(const std::string& code, const std::string& language_file);

    /** @param active_language The language that will be active. */
    bool Setup(const std::string& active_language);

  private:
    Font *current_font_;
    std::unordered_map<std::string, std::unique_ptr<Font>> fonts_;

    std::string default_language_;
    std::string active_language_;
    std::unordered_map<std::string, std::string> registered_languages_;
};

}  // namespace text
}  // namespace ugdk

#endif // UGDK_TEXT_MANAGER_H_
