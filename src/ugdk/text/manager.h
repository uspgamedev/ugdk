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
    Manager();
    ~Manager();

    bool Initialize();
    bool Release();

    TextBox* GetText(        const std::string& text);
    TextBox* GetText(        const std::string& text, const std::string& font, int width = -1);
    TextBox* GetTextFromFile(const std::string& path);
	TextBox* GetTextFromFile(const std::string& path, const std::string& font, int width = -1);

    Font* current_font() const { return current_font_; }
    Font* GetFont(const std::string& name) const;

	void AddFont(const std::string& name, const std::string& path, double size);

  private:
    Font *current_font_;
    std::unordered_map<std::string, std::unique_ptr<Font>> fonts_;
};

}  // namespace text
}  // namespace ugdk

#endif // UGDK_TEXT_MANAGER_H_
