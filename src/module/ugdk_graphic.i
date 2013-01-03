
%module ugdk_graphic

%include <module/export.swig>
%include <module/ownership.swig>
%include <module/widetypes.swig>
%include "std_string.i"
%include "std_vector.i"
%include "std_map.i"

%{

#include <ugdk/graphic/geometry.h>
#include <ugdk/graphic/drawable.h>
#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/font.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/graphic/textmanager.h>
#include <ugdk/graphic/videomanager.h>

%}

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_math") <ugdk/math/frame.h>

%ignore ugdk::graphic::Texture::CreateFromSurface(SDL_Surface* data);
%ignore ugdk::graphic::VideoManager::PushAndApplyModifier(const Geometry& apply);
%ignore ugdk::graphic::Font::GetLetterSize(wchar_t letter);
%ignore ugdk::graphic::SpritesheetData::frames() const;

%newobject ugdk::graphic::Font::GetLetterSize(wchar_t letter);
%newobject ugdk::graphic::Geometry::Compose(const Geometry* mod1, const Geometry* mod2);
%newobject ugdk::graphic::Geometry::Copy(const Geometry* mod2);
%newobject ugdk::graphic::TextManager::GetText(const std::wstring& text);
%newobject ugdk::graphic::TextManager::GetText(const std::wstring& text, const std::string& font, int width = -1);
%newobject ugdk::graphic::TextManager::GetTextFromFile(const std:: string& path);
%newobject ugdk::graphic::TextManager::GetTextFromFile(const std:: string& path, const std::string& font, int width = -1);
%newobject ugdk::graphic::Texture::CreateFromFile(const std::string& filepath);
%newobject ugdk::graphic::Texture::CreateRawTexture(int texture_width, int texture_height);

%rename(ComposeNew) ugdk::graphic::Geometry::Compose(const Geometry* mod1, const Geometry* mod2);
%rename(GetTextWithFont) ugdk::graphic::TextManager::GetText(const std::wstring& text, const std::string& font, int width = -1);

%include <ugdk/graphic/geometry.h>

%include <ugdk/graphic/drawable.h>

%import(module="ugdk_base") <ugdk/base/types.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_drawable") <ugdk/graphic/drawable/text.h> //this needs to go after the include drawable.h

%include <ugdk/graphic/texture.h>
%include <ugdk/graphic/font.h>
%include <ugdk/graphic/light.h>
enable_disown(ugdk::graphic::Node* new_child)
enable_disown(ugdk::graphic::Drawable* drawable)
%include <ugdk/graphic/node.h>
disable_disown(ugdk::graphic::Drawable* drawable)
disable_disown(ugdk::graphic::Node* new_child)
%include <ugdk/graphic/spritesheet.h>

%include <ugdk/graphic/textmanager.h>
%include <ugdk/graphic/videomanager.h>

namespace ugdk {
namespace graphic {
    export_class(Drawable)
    export_class(Texture)
    export_class(Font)
    export_class(Light)
    export_class(Geometry)
    export_class(Node)
    export_class(Spritesheet)
    export_class(TextManager)
    export_class(VideoManager)
}
}

confirm_exports(ugdk_graphic)
