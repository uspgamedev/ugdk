#ifndef HORUSEYE_FRAMEWORK_TEXTMANAGER_H_
#define HORUSEYE_FRAMEWORK_TEXTMANAGER_H_

#include <vector>
#include <map>
#include <ugdk/config/config.h>
#ifdef ISMAC
    #include "SDL_ttf.h"
#else
    #include <SDL/SDL_ttf.h>
#endif
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>

#define TEXT_MANAGER() ugdk::Engine::reference()->text_manager()

namespace ugdk {
namespace graphic {

using std::wstring;
class TextManager {
    public:
        TextManager() : current_font_(NULL) {}
        ~TextManager();

        bool Initialize();
        bool Release();

		Text* GetText(wstring text, wstring font = wstring(), int width = -1);
		Text* GetTextFromFile(wstring path, wstring font, int width = -1);
		void AddFont(wstring name, wstring path, int size, char ident, bool fancy);

    private:
		Font *current_font_;
		std::map<wstring,Texture**> font_images_;
		std::map<wstring,Font*> fonts_;
};

}  // namespace graphic
}  // namespace ugdk

#endif
