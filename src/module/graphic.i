
%module ugdk_graphic

%include <module/export.swig>
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%{

#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/font.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/modifier.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/videomanager.h>

%}

%import <module/math.i>
%import <module/base.i>
%import <module/action.i>

%ignore ugdk::graphic::Texture::CreateFromSurface(SDL_Surface* data);
%ignore ugdk::graphic::VideoManager::PushAndApplyModifier(const Modifier& apply);
%ignore ugdk::graphic::Font::GetLetterSize(wchar_t letter);

%newobject ugdk::graphic::Font::GetLetterSize(wchar_t letter);
%newobject ugdk::graphic::Modifier::Compose(const Modifier* mod1, const Modifier* mod2);
%newobject ugdk::graphic::Modifier::Copy(const Modifier* mod2);
%newobject ugdk::graphic::TextManager::GetText(const std::wstring& text);
%newobject ugdk::graphic::TextManager::GetText(const std::wstring& text, const std::string& font, int width = -1);
%newobject ugdk::graphic::TextManager::GetTextFromFile(const std:: string& path);
%newobject ugdk::graphic::TextManager::GetTextFromFile(const std:: string& path, const std::string& font, int width = -1);
%newobject ugdk::graphic::Texture::CreateFromFile(const std::string& filepath);
%newobject ugdk::graphic::Texture::CreateRawTexture(int texture_width, int texture_height);

%rename(ComposeNew) ugdk::graphic::Modifier::Compose(const Modifier* mod1, const Modifier* mod2);;

/*
graphic::Node has several methods that steals the ownership of the object
so that might give us trouble in the future...
check SWIG %delobject directive
*/

%include <ugdk/graphic/drawable.h>

%import <module/drawable.i> //this needs to go after the include drawable.h

%include <ugdk/graphic/texture.h>
%include <ugdk/graphic/font.h>
%include <ugdk/graphic/light.h>
%include <ugdk/graphic/modifier.h>
%include <ugdk/graphic/node.h>
%include <ugdk/graphic/spritesheet.h>
%include <ugdk/graphic/textmanager.h>
%include <ugdk/graphic/videomanager.h>

namespace ugdk {
namespace graphic {
    export_class(Drawable)
    export_class(Texture)
    export_class(Font)
    export_class(Light)
    export_class(Modifier)
    export_class(Node)
    export_class(Spritesheet)
    export_class(TextManager)
    export_class(VideoManager)
}
}

confirm_exports(ugdk_graphic)
