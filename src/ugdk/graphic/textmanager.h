#ifndef HORUSEYE_FRAMEWORK_TEXTMANAGER_H_
#define HORUSEYE_FRAMEWORK_TEXTMANAGER_H_

#include <vector>
#include <map>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>

#define TEXT_MANAGER() ugdk::Engine::reference()->text_manager()

namespace ugdk {
namespace graphic {

class TextManager {
  public:
    TextManager() : current_font_(nullptr) {}
    ~TextManager();

    bool Initialize();
    bool Release();

    Text* GetText(        const std::wstring& text);
    Text* GetText(        const std::wstring& text, const std::string& font, int width = -1);
    Text* GetTextFromFile(const std:: string& path);
	Text* GetTextFromFile(const std:: string& path, const std::string& font, int width = -1);

	void AddFont(const std::string& name, const std::string& path, int size, char ident, bool fancy);

  private:
	Font *current_font_;
	std::map<std::string, Texture**> font_images_;
	std::map<std::string, Font*> fonts_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
