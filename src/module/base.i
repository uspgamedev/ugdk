%module ugdk_base

%include <module/export.swig>
%include "std_string.i"
%include "std_map.i"

%{

#include <ugdk/base/types.h>
#include <ugdk/base/configuration.h>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcecontainer.h> //class template
#include <ugdk/base/texturecontainer.h>
#include <ugdk/base/resourcemanager.h>

#include <ugdk/graphic/texture.h>
#include <ugdk/graphic/spritesheet.h>
#include <ugdk/util/languageword.h>

%}

%include <ugdk/base/resourcecontainer.h> //class template
%template(ResourceContainer_Texture) ugdk::base::ResourceContainer<ugdk::graphic::Texture*>;

%template(ResourceContainer_Spritesheet) ugdk::base::ResourceContainer<ugdk::graphic::Spritesheet*>;
%template(ResourceContainer_LanguageWord) ugdk::base::ResourceContainer<ugdk::LanguageWord*>;

%import(module="ugdk_math") <ugdk/math/vector2D.h>
%import(module="ugdk_input") <ugdk/input.h>
%import(module="ugdk_time") <ugdk/time.h>
%import(module="ugdk_audio") <ugdk/audio.h>
%import(module="ugdk_action") <ugdk/action.h>
%import(module="ugdk_graphic") <ugdk/graphic.h>
%import(module="ugdk_util") <ugdk/util.h>

//%newobject ugdk::base::TextureContainer::Load(const std::string& filepath, const std::string& tag);
//%newobject ugdk::base::TextureContainer::Load(const std::string& filepath);

%include <ugdk/base/types.h>
%include <ugdk/base/configuration.h>
%include <ugdk/base/engine.h>
%include <ugdk/base/texturecontainer.h>
%include <ugdk/base/resourcemanager.h>

namespace ugdk {
	export_class(Color)
	export_class(Configuration)
	export_class(Engine)
	
namespace base {
    export_class(TextureContainer)
    export_class(AnimationLoader)
    export_class(ResourceManager)
}
}
 
confirm_exports(ugdk_base)
